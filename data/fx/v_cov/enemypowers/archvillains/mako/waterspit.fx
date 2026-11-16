#########################################################
##	
##

FxInfo

LifeSpan	200

Input  
	InpName	head
End

Input  
	InpName	chest
End

Input  
	InpName	UArmL
End

Input  
	InpName	UArmR
End

##################################
Condition
	On 	Time
	Time 	0

	Event
		EName	Spike
		Type	Local
		At	head
		
		Geom	LocalPivotz
		bhvr	behaviors/scale1a.bhvr	
	End

	
	Event
		EName	Prime
		Type	start
		At	spike
		Altpiv   1

		Bhvr	behaviors/Quillprojectile1.bhvr

		Part	:SplashbodyCore.part
		Part	:SplashBubbles.part
		Part	:SplashBubblesSmall.part
		Part	:SplashbodyCore2.part

#		Part	:SplashbodyMist.part
		Part	:SplashbodyCore3.part

		
		Magnet	Target
		Lookat	Targrt
	End
	
End


Condition
	On 	PrimeHit
	
	
	Event

		Type	start
		At	T_Chest
		Part	:SplashbodyHit.part
#		Part	:SplashBubbles.part
#		Part	:SplashBubblesSmall.part
#		Part	:SplashbodyCore2.part
#		Part	:SplashbodyCore3.part

	End

	Event
		EName 	Prime
		Type	Destroy
		

	End

End


End				