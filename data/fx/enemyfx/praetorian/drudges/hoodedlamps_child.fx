FXinfo

	Condition
		On time
		Time 0

		Event
			Ename Prop
			At Origin
			Type Positonly
			BhvrOverride
				scale .5 .5 .5
				pyrRotateJitter 13 128 8
				Spin 0 .01 0
			End
			Bhvr Behaviors/FadeOut.bhvr
			Geom FX_HoodedLamp_Brass
			#Anim FX_HoodedLamp
		End

		Event
			At Prop
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			Geom FX_HoodedLamp_Glass
		End

		Event
			At Prop
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.15 0
			End
			Part :HoodedLamp_flame.part
			Part :HoodedLamp_flame_Core.part
			Part :HoodedLamp_Glow_Core.part
			Part :HoodedLamp_Glow_Add.part
			Part :HoodedLamp_Glow_Trail.part
			Part :HoodedLamp_Glow.part
		End

		Event
			At Prop
			Type Local
			EName CandleAdd
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.015 0
			End
			Geom LampCandleAdd
		End
	End
End