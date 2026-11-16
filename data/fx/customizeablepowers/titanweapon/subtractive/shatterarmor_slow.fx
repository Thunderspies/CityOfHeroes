#########################################################
##	template

FxInfo

Lifespan 300

#################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_ShatterArmor_AttackSlow_01 100 100 .8
	End
End

Condition
	On	Time
	Time	65

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 2.0 -5
			PYRRotate	90 0 90
		End
		Part	:Air_Dust_Traveling.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 3
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.5 -5
			PYRRotate	0 90 180

		End
		Part	:Air_Dust_Traveling_Long.part
		Part	:Air_Dust_Traveling_Additive.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	67

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 5.0
		End
		Part	:Ground_Dust_Traveling.part
		Lifespan 7
	End
End

End

