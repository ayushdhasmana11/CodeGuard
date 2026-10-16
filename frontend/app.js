const express = require("express");
const app = express();

const ejsMate = require("ejs-mate");
const multer = require("multer");
const path = require("path");
const fs = require("fs");



//  temp
const { execFile } = require("child_process");
const { promisify } = require("util");

const execFileAsync = promisify(execFile);

const PROJECT_ROOT = path.join(__dirname, "..");
const ENGINE_PATH = path.join(PROJECT_ROOT, "main.exe");

//   temp

app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));


app.engine("ejs", ejsMate);


app.use(express.static(path.join(__dirname, "/public")));

// Middleware to read form data
app.use(express.urlencoded({ extended: true }));
   
const Submission = require("./models/listing.js");
const mongoose = require("mongoose");
const MONGO_URL = "mongodb://127.0.0.1:27017/CodeGuard";



// Configure file uploads
// temp
const ASSIGNMENTS_DIR = path.join(PROJECT_ROOT, "assignments");

fs.mkdirSync(ASSIGNMENTS_DIR, { recursive: true });

function getAssignmentFolder(assignment) {
  const folderName = assignment
    .trim()
    .toLowerCase()
    .replace(/[^a-z0-9_-]+/g, "-")
    .replace(/^-+|-+$/g, "");

  if (!folderName) {
    throw new Error("Invalid assignment name.");
  }

  return path.join(ASSIGNMENTS_DIR, folderName);
}

const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    try {
      const assignment = req.body.assignment;

      if (!assignment || !assignment.trim()) {
        return cb(new Error("Please enter an assignment name."));
      }

      const folder = getAssignmentFolder(assignment);

      fs.mkdirSync(folder, { recursive: true });

      cb(null, folder);
    } catch (err) {
      cb(err);
    }
  },

  filename: (req, file, cb) => {
    cb(null, `${Date.now()}-${path.basename(file.originalname)}`);
  }
});
// temp

const upload = multer({
  storage: storage,
  limits: {
    fileSize: 1 * 1024 * 1024 // 1 MB
  }
});
main()
.then(()=>{
  console.log("Connected to DB");
}).catch((err)=>{
  console.log(err);
});

async function main(){
  await mongoose.connect(MONGO_URL);  
}


// app.get("/testListing",async (req, res)=>{
//   let sampleListing = new Listing({
//     title: "My New villa",
//     description: "By the beach",
//     price: 1200,
//     location: "Calangute, Goa",
//     country: "India",
//   });
//   await sampleListing.save();
//   console.log("Sample was saved");
//   res.send("Successful testing");
// });
// NEW ROUTE
app.get("/listings/new", (req, res)=>{
  res.render("listings/submit.ejs");
});


// app.post("/listings", async(req, res) => {
//   try{
//     const newSubmission = new Submission(req.body.listing);
//     await newSubmission.save();
//     res.redirect("/listings");
//   }
//   catch(err){
//     console.log(err);
//     res.status(500).send("Error saving submission");
//   }
// });

// app.post("/listings", validateListing ,wrapAsync(async (req, res, next)=>{

// const newListing = new Listing(req.body.listing);

// await newListing.save();
//   res.redirect("/listings");

// }));

app.post("/listings", upload.single("codeFile"), async (req, res) => {
  let newSubmission;
  let isResubmission = false;
  let oldFilePath = null;

  try {
    if (!req.file) {
      return res.status(400).send("Please upload a source-code file.");
    }

    const { studentName, rollNumber, assignment, language, submissionId } = req.body;
    const extension = path.extname(req.file.originalname).toLowerCase();

    const expectedExtensions = {
      "C++": ".cpp",
      "C": ".c"
    };

    if (expectedExtensions[language] !== extension) {
      fs.unlink(req.file.path, () => {});
      return res.status(400).send(
        "The selected programming language does not match the file extension."
      );
    }

    if (
      !studentName?.trim() ||
      !Number.isSafeInteger(Number(rollNumber)) ||
      Number(rollNumber) < 1 ||
      !assignment?.trim() ||
      !language
    ) {
      fs.unlink(req.file.path, () => {});
      return res.status(400).send("Please provide valid submission details.");
    }

    // Find the existing record for a resubmission
    if (submissionId) {
      newSubmission = await Submission.findById(submissionId);

      if (!newSubmission) {
        fs.unlink(req.file.path, () => {});
        return res.status(404).send("Submission not found.");
      }

      isResubmission = true;
      oldFilePath = newSubmission.filePath;

    } else {
      // A normal submission creates a new record
      newSubmission = new Submission({
        studentName,
        rollNumber: Number(rollNumber),
        assignment,
        language,
        fileName: req.file.originalname,
        filePath: req.file.path
      });
    }

    // Save the new submission details
    newSubmission.studentName = studentName;
    newSubmission.rollNumber = Number(rollNumber);
    newSubmission.assignment = assignment;
    newSubmission.language = language;
    newSubmission.fileName = req.file.originalname;
    newSubmission.filePath = req.file.path;
    newSubmission.status = "processing";
    newSubmission.plagiarismPercentage = null;
    newSubmission.comparisonResults = [];
    newSubmission.engineError = null;

    await newSubmission.save();

    // Remove the old file so it is not compared against its replacement
    if (
      isResubmission &&
      oldFilePath &&
      path.resolve(oldFilePath) !== path.resolve(req.file.path) &&
      fs.existsSync(oldFilePath)
    ) {
      fs.unlinkSync(oldFilePath);
    }

//     res.send("Submission saved successfully!");
//   } catch (err) {
//     console.log(err);
//     if(req.file){
//       fs.unlink(req.file.path, () => {});
//     }
//     res.status(500).send("Error saving submission");
//   }
// });


const assignmentFolder = getAssignmentFolder(assignment);
    const { stdout } = await execFileAsync(
      ENGINE_PATH,
      [assignmentFolder],
      {
        cwd: PROJECT_ROOT,
        windowsHide: true,
        timeout: 30000,
        maxBuffer: 10 * 1024 * 1024
      }
    );

    console.log("CodeGuard report:\n", stdout);

    // 6. Extract comparisons for the uploaded file
    const uploadedFile = path.basename(req.file.path);
    const regex = /^(.+?)\s+<->\s+(.+?)\s+:\s+(\d+(?:\.\d+)?)%$/gm;
    const results = [];
    let match;

    while ((match = regex.exec(stdout)) !== null) {
      const file1 = match[1].trim();
      const file2 = match[2].trim();
      const score = Number(match[3]);

      if (file1 === uploadedFile) {
        results.push({
          comparedFileName: file2,
          similarityPercentage: score
        });
      } else if (file2 === uploadedFile) {
        results.push({
          comparedFileName: file1,
          similarityPercentage: score
        });
      }
    }

    // 7. Save similarity results
    newSubmission.comparisonResults = results;
    newSubmission.plagiarismPercentage = results.length
      ? Math.max(...results.map(result => result.similarityPercentage))
      : 0;

    newSubmission.status = "completed";
    newSubmission.engineError = null;

    await newSubmission.save();

    // 8. Display result
    res.render("listings/results.ejs", {
  submission: newSubmission
});

  } catch (err) {
    console.error("Submission or engine error:", err);

    if (newSubmission) {
      newSubmission.status = "failed";
      newSubmission.engineError = err.stderr || err.message;
      await newSubmission.save().catch(console.error);
    }

    res.status(500).type("text/plain").send(
      "CodeGuard analysis failed:\n" + (err.stderr || err.message)
    );
  }
});




app.get("/", (req, res)=>{
    res.render("listings/home.ejs");
});

/* Result history: show all saved submissions */
app.get("/listings/results", async (req, res) => {
  try {
    const submissions = await Submission.find({})
      .sort({ createdAt: -1 });

    res.render("listings/history.ejs", { submissions });
  } catch (err) {
    console.error(err);
    res.status(500).send("Could not load result history.");
  }
});

/* Individual report: show one submission */
app.get("/listings/:id/resubmit", async (req, res) => {
  try {
    const submission = await Submission.findById(req.params.id);

    if (!submission) {
      return res.status(404).send("Submission not found.");
    }

    res.render("listings/submit.ejs", { previous : submission });
  } catch (err) {
    console.error(err);
    res.status(500).send("Could not load the report.");
  }
});


app.get("/test-engine", async (req, res) => {
  try {
    const { stdout, stderr } = await execFileAsync(
      ENGINE_PATH,
      [],
      {
        cwd: PROJECT_ROOT,
        windowsHide: true,
        timeout: 30000,
        maxBuffer: 10 * 1024 * 1024,
      }
    );

    res.type("text/plain").send(
      "C++ engine executed successfully!\n\n" +
      stdout +
      (stderr ? "\nWarnings:\n" + stderr : "")
    );
  } catch (err) {
    console.error("Engine execution failed:", err);

    res.status(500).type("text/plain").send(
      "C++ engine failed.\n\n" +
      (err.stderr || err.message)
    );
  }
});


app.listen(3030, () => {
    console.log("Server is lisiting ot port 3030");
});