// const mongoose = require("mongoose");
// const Schema = mongoose.Schema;

// const listingSchema = new Schema({
//   Student Name: {
//     type: String,
//     required: true,
//   },
//   description: String,
//   price: Number,
//   location: String,
//   country: String
// }); 


// const Listing = mongoose.model("Listing", listingSchema);

// module.exports = Listing;




const mongoose = require("mongoose");

const submissionSchema = new mongoose.Schema(
  {
    studentName: {
      type: String,
      required: true,
      trim: true,
    },

    rollNumber: {
      type: Number,
      required: true,
      trim: true,
      min: 1,
      validate: {
        validator: Number.isSafeInteger,
        message: "Roll Number must be an Integer"
      },

    },

    assignment: {
      type: String,
      required: true,
      trim: true,
    },

    language: {
      type: String,
      required: true,
      enum: ["C++", "C", "Java"],
    },

    fileName: {
      type: String,
      required: true,
    },

    filePath: {
      type: String,
      required: true,
    },

    status: {
      type: String,
      enum: ["Pending", "Processing", "Completed", "Failed"],
      default: "Pending",
    },

    plagiarismPercentage: {
      type: Number,
      min: 0,
      max: 100,
      default: null,
    },
  },
  {
    timestamps: true,
  }
);

const Submission = mongoose.model("Submission", submissionSchema);

module.exports = Submission;