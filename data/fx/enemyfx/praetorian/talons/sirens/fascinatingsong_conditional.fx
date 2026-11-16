#
FxInfo

Lifespan 1000

###########################################################

	#Condition
		#On Time
		#Time 0

		#Event
			#At Origin
			#Type Local
			#Flags PowerLoopingSound
			#Bhvr behaviors/soundFade3.bhvr
			#Sound domination1_loop 40.0 40.0 0.6
			#LifeSpan 280
		#End

	#End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName HeadGlow
			Bhvr Behaviors/GenericParticleFade.bhvr
			#Part POWERS\Ability\DominationDots1.part
			Part ENEMYFX\Praetorian\Talons\Sirens\FascinationGlow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\FascinationNoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\FascinationNoteB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_Core.part
			#Part ENEMYFX\Praetorian\Talons\Sirens\Glow_CoreAdd.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Glow_InwardCoreAdd.part
			#Part POWERS\Ability\DominationSparklingDots.part
		End

		Event
			At SpadR
			Type Local
			EName RightUarmGlow
			Bhvr Behaviors/GenericParticleFade.bhvr
			#Part POWERS\Ability\DominationDots1.part
			Part ENEMYFX\Praetorian\Talons\Sirens\FascinationGlow.part
			#Part POWERS\Ability\DominationSparklingDots.part
		End

		Event
			At SpadL
			Type Local
			EName LeftUarmGlow
			Bhvr Behaviors/GenericParticleFade.bhvr
			#Part POWERS\Ability\DominationDots1.part
			Part ENEMYFX\Praetorian\Talons\Sirens\FascinationGlow.part
			#Part POWERS\Ability\DominationSparklingDots.part
		End

	End

	Condition
		On Time
		Time 0
		#Repeat 1

		Event
			At Chest
			Type PositOnly
			EName Anchor
		End

		Event
			At Anchor
			Type Local
			Ename RotRing
			Bhvr Behaviors/FadeInOut.bhvr
			BhvrOverride
				Scale 0.3 0.4 0.3
				Spin 0 -0.03 0
				PyrRotateJitter 10 0 10
				#StartJitter 1 1 1
			End
			Geom FX_MusicRing_02
			#LifeSpan 45
		End

		Event
			At RotRing
			Type Local
			Ename RotRing2
			Bhvr Behaviors/FadeInOut.bhvr
			BhvrOverride
				#Scale 0.3 0.5 0.3
			End
			Geom FX_MusicRing_02
			#LifeSpan 45
		End

	End

End
