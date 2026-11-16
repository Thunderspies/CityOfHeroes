#########################################################
##	SStreetSteeam01
##
FxInfo

Input  
	InpName	origin
End

Input  
	InpName	target
End
	
Input  
	Inpname	LarmL
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	WepL
End

Input  
	Inpname	T_Root
End

LifeSpan	350
###############################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	Hand1
		Type	Local 
		At	UarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		Sound DarkPit1 80 80 .78
				
		PMagnet	LarmL
		LifeSpan	27
	End
	
	Event
		EName 	Hand2
		Type	Local 
		At	LarmL
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	27
	End

End


Condition
	On	time
	time	18
		
	
	Event
		EName 	Prime
		Type	Start 
		At	WepL
		Bhvr	behaviors/MorterLobbprojectile.bhvr

		Part	:DarknessRapidTenticles_Subtractive.part	
		Part1	:DarknessRapidTenticles.part
		Part2	:DarknessRapidTenticlesCore.part
		Part3	:DarknessRapidCloud.part
		Part4	:DarknessRapidCloud2.part
		Part5	:SmallTenticlesHead.part
				
		Magnet	T_Root
		LookAt	T_Root
	End

End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		EName 	burst
		Type	start 
		At	T_Root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Sound grexplo2 80 80 1.0
		
	End	
	
	Event
		EName	darkskirt
		Type	start #Start
		At	T_ROOT
		
		Part	:SubtractiveFlash1.part
		Part	:CreateDarknessPuddle_Subtractive_Local.part
		Part1	:CreateDarknessPuddle_Subtractive.part
		Part2	:CreateDarknessPuddle2_Subtractive.part

		Sound forcefield5_loop 80 80 .7
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_ROOT

		Part1	:CreateDarknessTrail_Subtractive.part
		Part2	:CreateDarknessTrail2_Subtractive.part
		Part1	:CreateDarknessPuddleFlat_Subtractive.part
		Part2	:CreateDarknessPuddle2Flat_Subtractive.part
		Sound miasma_loop 80 80 .6
	End


	Event
		EName	ringBurst
		Type	start #Start
		At	T_ROOT
				
		Part4	:ThePitRing.Part
	End
End


End