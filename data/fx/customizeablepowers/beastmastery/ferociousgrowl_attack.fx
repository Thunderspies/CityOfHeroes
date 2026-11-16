###########################################################

FxInfo
Lifespan 70

###########################################################


Condition
	On Time
	Time 15

	Event
		At Origin
		Type Local
		BhvrOverride
			PYRRotate	0 0 0
		End
		CHILDFX :CHILD_FerociousGrowl_Attack.fx
	End

	Event
		At Root
		Type Local
		BhvrOverride
			PYRRotate	0 180 0
			PositionOffset	0.0 0.1 0.0
		End
		Part 	:Roar_Wind_Shockwave.part
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	25

	Event
		At Root
		Type Local
		BhvrOverride
			PYRRotate	0 180 0
			PositionOffset	0.0 0.1 0.0
		End
		Part 	:Roar_Wind_Shockwave_Core.part
		LifeSpan 20
	End
End


###########################################################

End