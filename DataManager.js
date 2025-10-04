// DataManager.js

function getAmplificationData() {
    return [
        {x:0,  y:0.05},
        {x:1,  y:0.06},
        {x:2,  y:0.08},
        {x:3,  y:0.10},
        {x:4,  y:0.15},
        {x:5,  y:0.20},
        {x:6,  y:0.25},
        {x:7,  y:0.30},
        {x:8,  y:0.40},
        {x:9,  y:0.55},
        {x:10, y:0.70},
        {x:11, y:0.85},
        {x:12, y:1.10},
        {x:13, y:1.35},
        {x:14, y:1.60},
        {x:15, y:1.90},
        {x:16, y:2.20},
        {x:17, y:2.60},
        {x:18, y:3.00},
        {x:19, y:3.40},
        {x:20, y:3.90},
        {x:21, y:4.40},
        {x:22, y:5.00},
        {x:23, y:5.50},
        {x:24, y:6.10},
        {x:25, y:6.80},
        {x:26, y:7.40},
        {x:27, y:8.00},
        {x:28, y:8.50},
        {x:29, y:9.00},
        {x:30, y:9.40},
        {x:31, y:9.70},
        {x:32, y:9.90},
        {x:33, y:10.10},
        {x:34, y:10.25},
        {x:35, y:10.40},
        {x:36, y:10.55},
        {x:37, y:10.70},
        {x:38, y:10.85},
        {x:39, y:11.00}
    ];
}

// --- Data untuk Standard Curve (Starting Quantity vs Ct) ---
function getStandardCurveData() {
    return [
        {sq: 1, ct: 35},
        {sq: 10, ct: 30},
        {sq: 100, ct: 25},
        {sq: 1000, ct: 21},
        {sq: 10000, ct: 18}
    ];
}

// --- Data untuk Summary Page ---
function getSummary() {
    return {
        status: "Finished",
        totalCycles: 30,
        totalTime: "120 s",
        ctValue: 18,
        maxFluorescence: "1520 RFU",
        minFluorescence: "100 RFU",
        meanFluorescence: "720 RFU",
        conclusion: "Positive Amplification"
    };
}

// --- Data Raw (Cycle, Fluorescence, Temperature) ---
function getRawData() {
    return [
        { cycle: 1,  fluorescence: 150,  temperature: 95 },
        { cycle: 2,  fluorescence: 200,  temperature: 60 },
        { cycle: 3,  fluorescence: 250,  temperature: 72 },
        { cycle: 4,  fluorescence: 300,  temperature: 95 },
        { cycle: 5,  fluorescence: 350,  temperature: 60 },
        { cycle: 6,  fluorescence: 400,  temperature: 72 },
        { cycle: 7,  fluorescence: 460,  temperature: 95 },
        { cycle: 8,  fluorescence: 520,  temperature: 60 },
        { cycle: 9,  fluorescence: 590,  temperature: 72 },
        { cycle: 10, fluorescence: 660,  temperature: 95 },
        { cycle: 11, fluorescence: 740,  temperature: 60 },
        { cycle: 12, fluorescence: 820,  temperature: 72 },
        { cycle: 13, fluorescence: 900,  temperature: 95 },
        { cycle: 14, fluorescence: 980,  temperature: 60 },
        { cycle: 15, fluorescence: 1060, temperature: 72 },
        { cycle: 16, fluorescence: 1140, temperature: 95 },
        { cycle: 17, fluorescence: 1220, temperature: 60 },
        { cycle: 18, fluorescence: 1300, temperature: 72 },
        { cycle: 19, fluorescence: 1380, temperature: 95 },
        { cycle: 20, fluorescence: 1460, temperature: 60 },
        { cycle: 21, fluorescence: 1540, temperature: 72 },
        { cycle: 22, fluorescence: 1620, temperature: 95 },
        { cycle: 23, fluorescence: 1700, temperature: 60 },
        { cycle: 24, fluorescence: 1780, temperature: 72 },
        { cycle: 25, fluorescence: 1860, temperature: 95 },
        { cycle: 26, fluorescence: 1940, temperature: 60 },
        { cycle: 27, fluorescence: 2020, temperature: 72 },
        { cycle: 28, fluorescence: 2100, temperature: 95 },
        { cycle: 29, fluorescence: 2180, temperature: 60 },
        { cycle: 30, fluorescence: 2260, temperature: 72 },
        { cycle: 31, fluorescence: 2340, temperature: 95 },
        { cycle: 32, fluorescence: 2420, temperature: 60 },
        { cycle: 33, fluorescence: 2500, temperature: 72 },
        { cycle: 34, fluorescence: 2580, temperature: 95 },
        { cycle: 35, fluorescence: 2660, temperature: 60 },
        { cycle: 36, fluorescence: 2740, temperature: 72 },
        { cycle: 37, fluorescence: 2820, temperature: 95 },
        { cycle: 38, fluorescence: 2900, temperature: 60 },
        { cycle: 39, fluorescence: 2980, temperature: 72 },
        { cycle: 40, fluorescence: 3060, temperature: 95 }
    ];
}
