#########################################################
##	template

FxInfo


Flags    InheritAlpha

Condition

	Event
		Ename	Lvent01
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		58 -10 26
		End
		
	End

	Event
		Ename	TargetNodeLeft
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -60 -126
		End
		
#		Part	:BlinkingLights.part
	End
End

Condition
	On	Cycle
	Time	2

	Event
		Ename	PowerStream01
		Type	start
		At	Lvent01
		bhvr	behaviors/projectile1.bhvr
		Part	:EnergyStreak.part
		
		Magnet	TargetNodeLeft
		LifeSpan	20

	End

End

End