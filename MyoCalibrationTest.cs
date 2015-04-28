using UnityEngine;
using System.Collections;

public class MyoCalibration : MonoBehaviour
{
   /* private const int numFvs = 8;
    private const int numDataPointsToAverage = 70;//Any way to make this a public variable so we can change it in unity instead of in code?  
    private int[] FistAverages = new int[numFvs];  //This is just where I'm storing the average data for a gesture
    private float[] StandardDeviations = new float[numFvs];
    private int iteratorForRawData = 0;

    private bool calibrationInProgress = false;
    private bool calibrationCompleted = false;
    private bool poseCheck = false;

    //These arrays are where I'm storing the raw data
    private int[][] rawData = new int[numFvs][]{new int[numDataPointsToAverage], new int[numDataPointsToAverage], 
        new int[numDataPointsToAverage], new int[numDataPointsToAverage],
        new int[numDataPointsToAverage], new int[numDataPointsToAverage],
        new int[numDataPointsToAverage], new int[numDataPointsToAverage]};

    public GUIText[] displayTexts = new GUIText[numFvs];
    public GUIText poseType;
    public float errorFactor = 25;

    // Use this for initialization
    void Start()
    {
        MYO_SDK_RAW.InitializeMYO();
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            Application.Quit();
        }

        MYO_SDK_RAW.RunRaw();

        if (calibrationInProgress) Calibrate1Frame();


        if (calibrationCompleted && !poseCheck)
        {           
            if (!checkPoseTypeFist()) return;
            poseCheck = true;           
            StartCoroutine(PrintPoseType());        
        }


        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            //This starts the calibration process
            calibrationInProgress = true;
            calibrationCompleted = false;
        }     
        
    }


    void Calibrate1Frame()
    {
        for (int i = 0; i < numFvs; i++)
            rawData[i][iteratorForRawData] = getFv(i);

        ++iteratorForRawData;

        displayTexts[0].text = "" + iteratorForRawData;

        if(iteratorForRawData >= numDataPointsToAverage)
        {
            calibrationCompleted = true;
            calibrationInProgress = false;
            iteratorForRawData = 0;
            CalculateRawData();
        }
    }

    void CalculateRawData()
    {
        for (int i = 0; i < numFvs; i++)
        {
            for (int j = 0; j < numDataPointsToAverage; j++)
                FistAverages[i] += rawData[i][j];

            FistAverages[i] /= numDataPointsToAverage;
            StandardDeviations[i] = CalculateStandardDeviation(rawData[i], FistAverages[i]);
            if (i == 0)
                Debug.Log(i + ": Average: " + FistAverages[i] + " STDeviation: " + StandardDeviations[i]);
        }

        for (int i = 0; i < numDataPointsToAverage; i++)
            Debug.Log("Data point " + i + " of oneRawData: " + rawData[0][i]);

        PrintAverages();
    }
    

    private IEnumerator PrintPoseType()
    {
        poseType.text = "YOU'Z A FIST MOTHA FUCKAAAA";
        yield return new WaitForSeconds(0.3f);
        poseCheck = false;
    }


    private float CalculateStandardDeviation(int[] dataPoints, int average)
    {
        float sum = 0;
        float tempDataPoint = 0;
        for (int i = 0; i < numDataPointsToAverage; i++)
        {
            tempDataPoint = (dataPoints[i] - average);
            sum = sum + (tempDataPoint * tempDataPoint) *errorFactor;
        }
        return Mathf.Sqrt(sum / average);
    }

    private void PrintAverages()
    {
        for (int i = 0; i < numFvs; i++)
        {
            displayTexts[i].text = FistAverages[i].ToString();
        }
    }

    private bool checkPoseTypeFist()
    {
        int numError = 0;
        for (int i = 0; i < numFvs; i++)
            if (!checkOneFv(getFv(i), i))
                ++numError;

        if (numError <= 2)
            displayTexts[1].text = "num of errors :" + numError;

        return (numError <= 2);          
    }


    private bool checkOneFv(int fv, int fvIdx)
    {
        if ((fv >= FistAverages[fvIdx] - StandardDeviations[fvIdx])
                    && (fv <= FistAverages[fvIdx] + StandardDeviations[fvIdx]))
            return true;
        else
        {
            poseType.text = "Failed at " + fvIdx + ": Raw: " + fv + " Average: " 
                + FistAverages[fvIdx] + " STDev: " + StandardDeviations[fvIdx];
            return false;
        }
    }


    public int getFv(int fvIdx)
    {
        switch(fvIdx)
        {
            case 0: return MYO_SDK_RAW.Fv0();
            case 1: return MYO_SDK_RAW.Fv1();
            case 2: return MYO_SDK_RAW.Fv2();
            case 3: return MYO_SDK_RAW.Fv3();
            case 4: return MYO_SDK_RAW.Fv4();
            case 5: return MYO_SDK_RAW.Fv5();
            case 6: return MYO_SDK_RAW.Fv6();
            case 7: return MYO_SDK_RAW.Fv7();
        }
        return -1;
    }
*/
   
}
