#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

#########################################################


Condition
	On	Cycle
	Time	15
	Chance	.45
	Random	1


	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	1.0 2.0 1.0
		End
		ChildFX	:Fireworks_Tendril.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	1.0 2.0 1.0
		End
		ChildFX	:Fireworks_Tendril_Yellow.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril_Yellow.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril_Yellow.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	1.0 2.0 1.0
		End
		ChildFX	:Fireworks_Tendril_Purple.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril_Purple.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril_Purple.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	1.0 2.0 1.0
		End
		ChildFX	:Fireworks_Tendril_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril2_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	1.0 2.0 1.0
		End
		ChildFX	:Fireworks_Tendril2_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril3_Small.fx
		Lifespan 90
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	2.0 2.0 2.0
		End
		ChildFX	:Fireworks_Tendril3_Small.fx
		Lifespan 90
	End
End

#############################################################

End