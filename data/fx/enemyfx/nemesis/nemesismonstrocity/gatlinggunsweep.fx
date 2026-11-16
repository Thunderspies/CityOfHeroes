
FxInfo
Lifespan 300

	Condition
		On time
		Time 68

		Event
			at LarmR
			type local
			Childfx :GatlingGunBurst.fx
			Lifespan 50
		End
	End


	Condition
		On Time
		Time 87

		Event
			Ename SpinAnchor
			At Root
			Type Local
			BhvrOverride
				spin		0 -0.03 0
			End
		End

		Event
			Ename GroundShots
			At SpinAnchor
			Type Local
			BhvrOverride
				PositionOffset 	3 0 5
			End
			ChildFX :GatlingGunSweep_CHILD.fx
			#Geom Geo_FX_Testsphere
			Lifespan 32
		End
	End

End