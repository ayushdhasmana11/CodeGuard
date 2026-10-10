const express = require("express");
const app = express();

const ejsMate = require("ejs-mate");


const multer = require("multer");
const path = require("path");
const fs = require("fs");
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
const INPUT_DIR = path.join(__dirname, "..", "data");

fs.mkdirSync(INPUT_DIR, { recursive: true });

const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    cb(null, INPUT_DIR);
  },
  filename: (req, file, cb) => {
    cb(null, `${Date.now()}-${file.originalname}`);
  }
});

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
  res.render("listings/new.ejs");
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
  try {
    if (!req.file) {
      return res.status(400).send("Please upload a source-code file.");
    }

    const newSubmission = new Submission({
      studentName: req.body.studentName,
      rollNumber: Number(req.body.rollNumber),
      assignment: req.body.assignment,
      language: req.body.language,
      fileName: req.file.originalname,
      filePath: req.file.path
    });

    await newSubmission.save();

    res.send("Submission saved successfully!");
  } catch (err) {
    console.log(err);
    res.status(500).send("Error saving submission");
  }
});


app.get("/", (req, res)=>{
    res.send("Posrt");
});

app.listen(3030, () => {
    console.log("Server is lisiting ot port 3030");
});