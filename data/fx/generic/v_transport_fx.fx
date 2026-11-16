
FxInfo
Lifespan 3650


	Condition
		On time
		Time 0

		Event
			Ename 	EngineSounds
			Type 	Local
			At 	Root
			ChildFX	ENEMYFX\V_Arachnos\Flyer\FX_Flyer_engine.fx
		End

		Event
			Type	Local
			At	Root
			ChildFX	ENEMYFX\V_Arachnos\Flyer\FX_Flyer_Lights.fx
		End

		Event
			Type	Local
			At	Root
			ChildFX	ENEMYFX\V_Arachnos\Critter_Flyer\FX_Flyer_Engine_Child.fx
		End

	End

	Condition
		On time
		Time 30

		Event
			Ename 	EngineSounds
			Type 	Local
			At 	Root
			BhvrOverride
				Scale .1 .1 .1
			End
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust.fx
			Lifespan 50
		End
	End

	Condition
		On time
		Time 30

		Event
			Ename 	EngineSounds
			Type 	Local
			At 	Root
			BhvrOverride
				Scale .1 .1 .1
			End
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust_Scaling.fx
			Lifespan 3520
		End
	End

	Condition
		On time
		Time 3545

		Event
			Type 	Local
			At 	Root
			BhvrOverride
				Scale .1 .1 .1
			End
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust.fx
			Lifespan 50
		End

		Event
			Type 	Local
			At 	Root
			BhvrOverride
				Scale .1 .1 .1
			End
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust.fx
			Lifespan 50
		End
	End


End