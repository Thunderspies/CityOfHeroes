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
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End

Input  
	Inpname	WepR
End

Input  
	Inpname	T_Root
End

LifeSpan	350
###############################################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		Sound DarkPit1 80 80 .78
				
		PMagnet	LarmR
		LifeSpan	27
	End
	
	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkBlastTenticle.part
		Part3	:DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	27
	End

End


Condition
	On	time
	time	33
		
	
	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/MorterLobbprojectile.bhvr
			
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
		
		Part1	:CreateDarknessPuddle.part
		Part2	:CreateDarknessPuddle2.part
		Sound forcefield5_loop 80 80 .7
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_ROOT

		Part1	:CreateDarknessTrail.part
		Part2	:CreateDarknessTrail2.part
		Part3	:CreateDarknessPuddleFlat.part
		Part4	:CreateDarknessPuddle2Flat.part
		Part5	:DarkTarRing2.Part
		Sound miasma_loop 80 80 .6
	End

#	Event
#		EName	ringBurst
#		Type	start #Start
#		At	T_ROOT
#				
#		Part4	:ThePitRing.Part
	End



Condition
	On	cycle
	Time	19

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_root

		Bhvr	:splat.bhvr
		splat	Generic_Ring.tga
		LifeSpan	30
	End


End



End			