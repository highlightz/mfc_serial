#define _AFXDLL

#include <math.h>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

#include "stdafx.h"
#include "SerialPort.h"

void genControls( vector< float >& angles, vector< float >& speeds )
{
	// Controls length
	const int numOfControls = 10;

	// Controls ranges
	const float maxAngle =  0.7f;
  	const float minAngle = -0.7f;
	const float maxSpeed =  0.7f;

	// Specify angles
	angles[0] =  0.8f;
  	angles[1] = -0.8f;
  	angles[2] =  0.8f;
  	angles[3] = -0.8f;
  	angles[4] =  0.8f;

  	angles[5] = -0.8f;
	angles[6] =  0.8f;
  	angles[7] = -0.8f;
  	angles[8] =  0.8f;
  	angles[9] = -0.8f;

	// Limit the angle controls to a safe range
  	for ( int i = 0; i < numOfControls; i++ )
  	{
    		if ( angles[i] > maxAngle )
    		{
      			angles[i] = maxAngle;
    		}

    		if ( angles[i] < minAngle )
    		{
      			angles[i] = minAngle;
    		}
  	}

	// Compute speeds according to specified angles
  	for ( int i = 0; i < numOfControls; i++ )
  	{
    		const float p = 10.0f;
    		speeds[i] = maxSpeed / sqrt( fabs( p * angles[i] ) + 1.0f );
  	}
}

void genSend( float angle, float speed, char* send )
{
	send[0] = 0xff;
	send[1] = 0xfe;

	// Speed control
	const int *p = (int *)&speed;
	send[5] = (*p>>24)&0xff;
	send[4] = (*p>>16)&0xff;
	send[3] = (*p>>8)&0xff;
	send[2] = (*p>>0)&0xff;

	// Angle control
	p = (int *)&angle;
	send[9] = (*p>>24)&0xff;
	send[8] = (*p>>16)&0xff;
	send[7] = (*p>>8)&0xff;
	send[6] = (*p>>0)&0xff;
}

void writeToPort( char* send )
{
	try
	{
		CSerialPort port;
		port.Open(  2, 115200, CSerialPort::NoParity, 8, CSerialPort::OneStopBit, CSerialPort::XonXoffFlowControl );
		
		//std::cout << "Done sending!" << std::endl;
		port.Write( send, 10 );

		port.Flush(  );

		port.Close(  );
	}
	catch( CSerialException *pEx )
	{
		TRACE( _T( "Handle Exception, Message:%s\n" ), pEx->GetErrorMessage(  ) );
		pEx->Delete(  );
	}
}

// This function sends a series of controls to start the car.
void makeStart( )
{
	// Controls length
	const int numOfStartControls = 20;
	vector< float > startAngles(numOfStartControls);
	vector< float > startSpeeds(numOfStartControls);
	const DWORD startControlPeriod = 500;

	// Specify angles and speeds
	// Send empty controls for a few periods
	startAngles[0] = 0.00f;    startSpeeds[0] = 0.00f;
	startAngles[1] = 0.00f;    startSpeeds[1] = 0.00f;
	startAngles[2] = 0.00f;    startSpeeds[2] = 0.00f;
	startAngles[3] = 0.00f;    startSpeeds[3] = 0.00f;
	startAngles[4] = 0.00f;    startSpeeds[4] = 0.00f;
	startAngles[5] = 0.00f;    startSpeeds[5] = 0.00f;
	startAngles[6] = 0.00f;    startSpeeds[6] = 0.00f;
	startAngles[7] = 0.00f;    startSpeeds[7] = 0.00f;
	startAngles[8] = 0.00f;    startSpeeds[8] = 0.00f;
	startAngles[9] = 0.00f;    startSpeeds[9] = 0.00f;
	
	// Send a few small speed controls to make it go straight ahead
	startAngles[10] = 0.00f;    startSpeeds[10] = 0.10f;
	startAngles[11] = 0.00f;    startSpeeds[11] = 0.10f;
	startAngles[12] = 0.00f;    startSpeeds[12] = 0.10f;
	startAngles[13] = 0.00f;    startSpeeds[13] = 0.10f;
	startAngles[14] = 0.00f;    startSpeeds[14] = 0.10f;
	startAngles[15] = 0.00f;    startSpeeds[15] = 0.10f;
	startAngles[16] = 0.00f;    startSpeeds[16] = 0.10f;
	startAngles[17] = 0.00f;    startSpeeds[17] = 0.10f;
	startAngles[18] = 0.00f;    startSpeeds[18] = 0.10f;
	startAngles[19] = 0.00f;    startSpeeds[19] = 0.10f;

	// Send the controls to port
	char send[10];  // Signal to be sent to serial port

	for ( int i = 0; i < numOfStartControls; i++ )
	{
		genSend( startAngles[i], startSpeeds[i], send );
		
		writeToPort( send );

		cout << startAngles[i] << ", " << startSpeeds[i] << endl;

		Sleep( startControlPeriod );
	}

	cout << "Started!" << endl;
	// When in here, it should have an initial speed and zero angle
}

// This function sends a series of controls to stop the car.
// The changes of controls should be smooth.
void makeStop( )
{
	// Controls length
	const int numOfStopControls = 7;
	vector< float > stopAngles(numOfStopControls);
	vector< float > stopSpeeds(numOfStopControls);

	const DWORD stopControlPeriod = 500;

	// Specify angles and speeds
	// TODO: a better way is to loop up the current control state,
	// and then to determine a reasonal initial control angle.
	stopAngles[0] = 0.30f;    stopSpeeds[0] = 0.30f;
	stopAngles[1] = 0.25f;    stopSpeeds[1] = 0.25f;
	stopAngles[2] = 0.20f;    stopSpeeds[2] = 0.20f;
	stopAngles[3] = 0.15f;    stopSpeeds[3] = 0.15f;
	stopAngles[4] = 0.10f;    stopSpeeds[4] = 0.10f;
	stopAngles[5] = 0.05f;    stopSpeeds[5] = 0.05f;
	stopAngles[6] = 0.00f;    stopSpeeds[6] = 0.00f;

	// Send the controls to port
	
	char send[10];  // Signal to be sent to serial port

	for ( int i = 0; i < numOfStopControls; i++ )
	{
		genSend( stopAngles[i], stopSpeeds[i], send );
		
		writeToPort( send );

		cout << stopAngles[i] << ", " << stopSpeeds[i] << endl;

		Sleep( stopControlPeriod );
	}

	cout << "Stopped!" << endl;
}

int main(  )
{
	makeStart( );
	
	// Generate controls
	const int numOfControls( 10 );
	vector< float > angles( numOfControls );
	vector< float > speeds( numOfControls );
	genControls( angles, speeds );

	// Signal to be sent to serial port
	char send[10];

	const DWORD controlPeriod = 3000;
	for ( int i = 0; i < numOfControls; i++ )
	{
		genSend( angles[i], speeds[i], send );
		cout << "angle: " << angles[i] << ",	speed: " << speeds[i] << endl << endl;
		
		writeToPort( send );

		Sleep( controlPeriod );
	}

	makeStop( );
	return 0;
}

/*
指令输出实例:
Speed       Angle      Output
1.0f        1.0f       FF FE 3F 80 00 00 3F 80 00 00
1.0f        -1.0f      FF FE 3F 80 00 00 BF 80 00 00
-1.0f       1.0f       FF FE BF 80 00 00 3F 80 00 00
-1.0f       1.0f       FF FE BF 80 00 00 BF 80 00 00
*/
