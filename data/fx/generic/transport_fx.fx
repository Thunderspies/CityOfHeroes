
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

	End

	Condition
		On time
		Time 0

		Event
			Type 	Local
			At 	Root
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust_Scaling.fx
			Lifespan 3600
		End

		Event
			Type 	Local
			At 	Root
			ChildFX	ENEMYFX\V_LongBow\LB_SkiffIntake.fx
			Lifespan 60
		End

		Event
			Type 	Local
			At 	Root
			ChildFX	ENEMYFX\V_LongBow\LB_SkiffThruster.fx
			Lifespan 60
		End
	End

	Condition
		On time
		Time 3575

		Event
			Type 	Local
			At 	Root
			ChildFX	ENEMYFX\V_LongBow\LB_SkiffIntake.fx
			#Lifespan 15
		End

		Event
			Type 	Local
			At 	Root
			ChildFX	ENEMYFX\V_LongBow\LB_SkiffThruster.fx
			#Lifespan 15
		End
	End

	Condition
		On time
		Time 3600

		Event
			Type 	Local
			At 	Root
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust.fx
			Lifespan 15
		End

		Event
			Type 	Local
			At 	Root
			ChildFX	Vehicles\Helicopter\Helicopter_Hover_GroundDust.fx
			Lifespan 15
		End
	End


End