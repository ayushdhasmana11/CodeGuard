const mongoose = require("mongoose");

const comparisonResultSchema = new mongoose.Schema({
  comparedFileName: {
    type: String,
    required: true,
  },
  similarityPercentage: {
      type: Number,
      min: 0,
      max: 100,
      required: true,
    },
  },
    {_id: false},
  );


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
      enum: ["pending", "processing", "completed", "failed"],
      default: "pending",
    },

    plagiarismPercentage: {
      type: Number,
      min: 0,
      max: 100,
      default: null,
    },

    comparisonResults: {
      type: [comparisonResultSchema],
      default: [],
    },

    engineError: {
      type: String,
      default: null
    }
  },
  {
    timestamps: true,
  }
);

const Submission = mongoose.model("Submission", submissionSchema);

module.exports = Submission;
