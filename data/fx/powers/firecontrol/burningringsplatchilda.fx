#########################################################
##	FireRing
##
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		EName 	ring
		Type	local 
		At	Root
		BhvrOverride
			PyrRotate	0 360 0
			PyrRotateJitter	0 360 0
			Spin		0 1 0
			SpinJitter	0 .3 0
		End
			
	End

	Event
		EName 	ring1
		Type	posit
		At	ring
		BhvrOverride
			Spin		0 1 0
			SpinJitter	0 .3 0
			PositionOffset	3.5 0 0
		End
			
	End
End

Condition
	On 	Cycle
	Time 	3

	Event
		EName 	ringHookup
		Type	local 
		At	ring1
#		Bhvr	:FireCircleSplat.bhvr
#		splat	smoke.tga
		lifeSpan	5
		
		BhvrOverride
			Spin		0 1 0
			SpinJitter	0 .3 0
			PositionOffset	3.5 0 0
		End

	End


End


Condition
	On 	Cycle
	Time 	3

	Event
		Type	start 
		At	ringHookup
		Bhvr	:FireCircleSplata.bhvr
		splat	waterSurf.tga
		lifeSpan	5
		
#		BhvrOverride
#			Spin		0 1 0
#			SpinJitter	0 .3 0
#			PositionOffset	3.5 0 0
#		End

	End


End

End			