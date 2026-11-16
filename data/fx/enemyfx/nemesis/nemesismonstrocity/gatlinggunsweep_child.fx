
FxInfo
Lifespan 2000

	Condition
		On Cycle
		Time 4
		Repeat 4

		Event
			at Origin
			type local
			BhvrOverride
				StartJitter 1.5 0 1.5
			End
			ChildFX ENEMYFX\Signature\duray\CHILD_CloseAirSupport.fx
			Lifespan 30
		End
	End

End