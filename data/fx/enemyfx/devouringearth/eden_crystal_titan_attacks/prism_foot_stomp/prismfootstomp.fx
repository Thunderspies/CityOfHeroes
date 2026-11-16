#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

lifespan 100

######################################################################


Condition	
	On	Time
	Time	22

	Event	
		Type	Local
		At	Origin
		Sound titanprismblast 90 90 .8
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UlegR
		Part1	:StrengthFootGlows.part
		PMagnet	LlegR
		Sound footstomp2 100.0 100.0 .85
	End
	
	Event
		EName	Streak2
		Type	local
		At	UlegL
		Part1	:StrengthFootGlows.part
		PMagnet	LlegL
	End

	Event
		EName	Streak3
		Type	local
		At	LlegR
		Part1	:StrengthLowerFootGlows.part
		PMagnet	FootR
	End
	
	Event
		EName	Streak4
		Type	local
		At	LlegL
		Part1	:StrengthLowerFootGlows.part
		PMagnet	FootL
	End

End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	FootR
		Part	:LightBurst.part
		Part	:LightBurstFlat.part
		Part	:Color_Beam_Red.part
		Part	:Color_Beam_Yellow.part
		Part	:Color_Beam_Blue.part
		Part	:Color_Beam_Green.part
		Part	:Color_Beam_Red.part
		Part	:Color_Beam_Yellow.part
		Part	:Color_Beam_Blue.part
		Part	:Color_Beam_Green.part
		Part	:Color_Beam_Red.part
		Part	:Color_Beam_Yellow.part
		Part	:Color_Beam_Blue.part
		Part	:Color_Beam_Green.part
		Part	:Color_Beam_Red.part
		Part	:Color_Beam_Yellow.part
		Part	:Color_Beam_Blue.part
		Part	:Color_Beam_Green.part
		Bhvr	Behaviors/CameraShake01.bhvr
	End

End

Condition
	On 	Time
	Time 	29
	Event
		EName	flash2
		Type	start
		At	FootR
		Part	:AOEflash1.part
		Part1	:AOEflash2.part
		Part2	:AOEflash3.part
	
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	Streak1
		Type	Destroy
		
	End
	
	Event
		EName	Streak2
		Type	Destroy
		
	End

	Event
		EName	Streak5
		Type	Destroy
		
	End

	Event
		EName	Streak6
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName	Streak3
		Type	Destroy
		
	End

	Event
		EName	Streak4
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName	all
		Type	Destroy
		
	End

End


End
#########################################################

