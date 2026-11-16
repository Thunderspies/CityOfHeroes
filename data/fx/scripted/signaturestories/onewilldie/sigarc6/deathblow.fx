#
FxInfo


#LifeSpan 200


###########################################################

	Condition
		On Time
		Time 8

		Event
			At Hips
			Type Local
			BhvrOverride
				PositionOffset 0 0.5 1.4
				PyrRotate 180 0 0
				Scale 0.6 0.6 0.6
			End
			Geom HighDamageArrow
		End

		Event
			At Hips
			Type Local
			ChildFx :BodySmoke.fx
			EName TPAnchor
			BhvrOverride
				PositionOffset 0 0.5 0
			End
			LifeSpan 100
		End

		Event
			At TPAnchor
			Type Start
			Part :Beacon_Border.part
			Part :Beacon_Swirls.part
			Part :Beacon_Swirls_Additive.part
			LifeSpan 1
		End

		Event
			At TPAnchor
			Type Start
			EName SpinAnchor
			Part :Beacon_Core.part
			Part :Beacon_Core_Glow.part
		End

	End

	Condition
		On Time
		Time 11

		Event
			At TPAnchor
			Type Start
			Part :Rays_Explode.part
			Part :Streaks_Explode_Detached.part
			Part :Streaks_Explode_Short.part
		End

		Event
			At TPanchor
			Type Local
			Part :Smoke_Aura.part
		End

	End

	Condition
		On Time
		Time 11

		Event
			At TPanchor
			Type Local
			Part :Smoke_Gusher.part
		End

	End

End
