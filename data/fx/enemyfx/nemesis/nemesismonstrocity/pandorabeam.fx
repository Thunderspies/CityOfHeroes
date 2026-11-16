#                                                          
FxInfo

	LifeSpan 200


###########################################################
Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	PandoraBeam_01 100 100 .8
	End
End

	Condition
		On Time
		Time 22

		Event
			At Head
			Type Create
			EName Anchor
			Inherit Position Rotation #
			Update Position Rotation #
		End

		Event
			At Hips
			Type Local
			EName ShakeAnchor
			Bhvr behaviors/CameraShake_Strong30.bhvr
			BhvrOverride
				#Shake 0.3
				#ShakeFallOff 0.001
				ShakeRadius 240
			End
			LifeSpan 30
		End

		Event
			At Anchor
			Type Local
			EName BeamAnchor
			BhvrOverride
				PyrRotate -125 0 0
			End
		End

		Event
			At BeamAnchor
			Type Start
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Box_PowerSplode_Streaks.part
			Part :Box_PowerSplode_Streaks2.part
			Part :Box_PowerSplode_Clouds_Lingering.part
			#Part :Box_PowerSplode_Shockwave.part
			Part :Box_PowerSplode_Flash01.part
			Part :Box_PowerSplode_Flash02.part
			Part :Box_PowerSplode.part
			LifeSpan 12
		End

		Event
			At BeamAnchor
			Type Start
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Nuke_Fire_Rising_Column.part
			Part :Nuke_Shockwave.part
			LifeSpan 5
		End

		Event
			At BeamAnchor
			Type Start
			Bhvr Behaviors\GenericParticleFade.bhvr
			#Part :FlareUp.part
			Part :UpSwooshes.part
			Part ENEMYFX\Nemesis\NemesisMonstrocity\PlasmaBeams.part
			Part :BlackMatter_Rising.part
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 27

		Event
			At BeamAnchor
			Type Start
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Nuke_Smoke_Column.part
			Part :Nuke_Smoke_Column_Add.part
			Part :Nuke_Smoke_Cloud_Lingering.part
			Part :Box_PowerSplode_Clouds.part
			Part :Nuke_Smoke_Column_Lingering.part
			LifeSpan 5
		End

	End

End
