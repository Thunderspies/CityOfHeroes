#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	FootL
End

Input  
	Inpname	FootR
End

Input  
	InpName	UlegR
End

Input  
	InpName	UlegL
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Input  
	Inpname	mystic
End

lifespan 100

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UlegR
		Part1	:StrengthFootGlows.part
		PMagnet	LlegR
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


	Event
		EName	Streak5
		Type	local
		At	FootR
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
		Sound footstomp2 100.0 100.0 .85
	End

	Event
		EName	Streak6
		Type	local
		At	FootL
		Part1	:StrengthStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	FootR  ##mystic
		Part1	:AOEflash.part
		Part2	:AOEflash2.part
		Part3	:FootStompStreaks.part
		Part4	:LightBurst.part
		Part5	:LightBurstFlat.part

		Bhvr	Behaviors/CameraShake01.bhvr
	End

End

Condition
	On 	Time
	Time 	29
	Event
		EName	flash2
		Type	start
		At	FootR   ##mystic
		
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

