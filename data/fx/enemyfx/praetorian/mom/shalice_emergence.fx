#
FxInfo

	Input
		InpName Origin
	End
Lifespan 200

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_ShaliceEmergence_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
	#		ChildFx :Possesion_BodyAura_Thick.fx
			EName auraFlare2
			LifeSpan 75
		End

	End

	Condition
		On Time
		Time 88

		Event
			At Chest
			Type Local
			EName auraFlare2
	#		Part :Rays_Explode.part
	#		Part :Streaks_Explode_Detached.part
	#		Part :Streaks_Explode_Short.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Start
			Bhvr behaviors\fadein.bhvr
			#Sound Apparitions_BossAura_Loop 100 100 0.5
		End

		Event
			At Hips
			Type Start
			EName SpinAnchor
			Bhvr behaviors\fadeOut.bhvr
			BhvrOverride
				Spin 0.0 0.35 0.0
			End
			LifeSpan 60
		End

		Event
			At Hips
			Type Start
			EName SpinAnchor2
			Bhvr behaviors\fadeOut.bhvr
			BhvrOverride
				Spin 0.0 0.2 0.0
			End
			LifeSpan 60
		End

		Event
			At Hips
			Type Start
			EName SpinAnchor3
			Bhvr behaviors\fadeOut.bhvr
			BhvrOverride
				Spin 0.0 0.05 0.0
			End
			LifeSpan 60
		End

		Event
			At Hips
			Type StartPositOnly
			Bhvr behaviors\fadein.bhvr
			BhvrOverride
				PositionOffset 0 0.1 0
				FadeInLength 100
			End
			Part :Dust_Ground_Circle.part
			Part :Fog_Ground_Circle.part
			Part :DustDevil_Dust_Core.part
			LifeSpan 60
		End

		Event
			At SpinAnchor
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 2.0 0.0
				FadeInLength 100
			End
			Part ENEMYFX\Praetorian\MoM\Dust_Ground_Spinning_Hilight.part
			Part :Dust_Ground_Spinning.part
			LifeSpan 60
		End

		Event
			At SpinAnchor2
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 8.0 0.0
			End
			Part :Dust_Ground_Spinning_Outer.part
			LifeSpan 60
		End

		Event
			At SpinAnchor3
			Type StartPositOnly
			BhvrOverride
				PositionOffset 0.0 12.0 0.0
			End
			Part :Dust_Thin_Rising.part
			LifeSpan 60
		End

	End

End
