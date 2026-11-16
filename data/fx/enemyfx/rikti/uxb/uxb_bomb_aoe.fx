

FxInfo

#########################################################
##	AUDIO
#########################################################










###########################################################
##Energy Arms
#########################################

Condition
	
	Event
		EName	SpinningTarget
		Type	local
		At	root
		geom	rikti_UXB
		BhvrOverride
			FadeOutLength		220
#			Spin		0 .05 0
#			SpinJitter	0 .03 0
		End

	End

	Event
		EName	SpinningTargetNode
		Type	local
		At	SpinningTarget
		
		BhvrOverride
			FadeOutLength		220
			StartJitter	42.5 3 42.5
			PositionOffset	0 7 0
		End

##########################################################################	

	End
End

Condition
	On	Cycle
	Time	35
	Chance	.6

	
	Event
		Type	start
		At	SpinningTarget
		
		BhvrOverride
			
			FadeOutLength		220

		End

		ChildFx	:Energy_Bolt.fx

#		Magnet	origin
		LifeSpan	35
	End
End

Condition
	On	Cycle
	Time	20
	Chance	.2

	
	Event
		Type	start
		At	SpinningTarget
		
		BhvrOverride
			
			FadeOutLength		220

		End

		ChildFx	:Energy_Bolt.fx

#		Magnet	origin
		LifeSpan	35
	End
End

###########################################################
##AOE VFX
#########################################

Condition
	On 	cycle
	Time 	15
	Repeat	2
	
	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Smoke.bhvr
		Splat	smoke2.tga
		LifeSpan	35

	End

End

Condition
	On 	Time
	Time 	3
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Smoke.bhvr
		Splat	smoke2.tga


	End

End

Condition
	On 	Time
	Time 	5
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Smoke.bhvr
		Splat	smoke2.tga


	End

End

Condition
	On 	Time
	Time 	6
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Smoke.bhvr
		Splat	smoke2.tga

	End

End

Condition
	On 	Time
	Time 	8
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root

		Bhvr	:AOE_Smoke.bhvr
		Splat	smoke2.tga


	End

End

##########################################################
##Smoke
################################################

Condition
	On 	cycle
	Time 	2
	Repeat	1
	
	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Energy.bhvr
		Splat	smoke4.tga
		LifeSpan	35

	End

End

Condition
	On 	Time
	Time 	3
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Energy.bhvr
		Splat	smoke4.tga


	End

End

Condition
	On 	Time
	Time 	5
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Energy.bhvr
		Splat	smoke4.tga


	End

End

Condition
	On 	Time
	Time 	6
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root
		
		Bhvr	:AOE_Energy.bhvr
		Splat	smoke4.tga

	End

End

Condition
	On 	Time
	Time 	8
	Repeat	2

	Event
		EName	crack
		Type	start
		At	root

		Bhvr	:AOE_Energy.bhvr
		Splat	smoke4.tga


	End

End

End