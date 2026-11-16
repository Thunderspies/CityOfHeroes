#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

Input  
	InpName	Origin
End

Input  
	InpName	Target
End    

Input  
	Inpname	WepR
End


#########################################################
#####    Audio 
#########################################################

Condition
	On 	Time
	Time 	25

	Event	
		Type	Local
		At	Origin
		Sound Sootthrow 70 70 .6
	End

End




#########################################################

Condition
	On 	Time
	Time 	0

#	Event
#		EName 	geomtemp
#		Type	Local 
#		At	WepR
#		geom	GEO_WepR_Staff_5
#		
#	End

	Event
		EName 	node
		Type	Local 
		At	WepR
		geom	Staff_05
		
	End
End

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime
		Type	Start 
		At	node
		altpiv	1
		
		Part2	Powers/Darknesscontrol/BeamRapidTenticlesA.part
		Part1	Powers/Darknesscontrol/DarkBlastPuddleTrailA.Part
		Part3	Powers/Darknesscontrol/BeamRapidCloud2A.part
		Part4	Powers/Darknesscontrol/MoonBeamEmberA.part
		Part5	Powers/Darknesscontrol/MoonBeamEmberLargeA.part
		

		Bhvr	behaviors/Fastprojectile6.bhvr
		Magnet	Target
		LookAt	Target
						
	End

	Event
		EName 	Prime2
		Type	Start 
		At	node
		altpiv	1

		Bhvr	behaviors/Fastprojectile6.bhvr
		Magnet	Target
		LookAt	Target
		Part1	Powers/Darknesscontrol/MoonBeamRapidCloudA.part
		Part2	Powers/Darknesscontrol/BeamRapidTenticlesHeadA.part
						
	End


End


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End

	Event
		EName 	HitFake
		Type	local 
		At	target
		Part1	Powers/Darknesscontrol/Blast2.part
		#Part2	Powers/Darknesscontrol/Blast.part
			LifeSpan	20			
	End
	
	

End

Condition
	On 	Time
	Time	50

	Event
		EName 	HitFake
		Type	Destroy

	End
End


End	


			