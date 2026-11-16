#
FxInfo

	Flags InheritAlpha

	LifeSpan 440


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_ShaliceDefeat_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Hips
			Type Local
			Flags AdoptParentEntity
			#Bhvr behaviors\faeriefadeout.bhvr
			LifeSpan 170
		End

		Event
			At Hips
			Type Local
			EName InitParticles
			Part ENEMYFX\Praetorian\MoM\Build_Up_Scatter_Glows_02.part
			Part ENEMYFX\Praetorian\MoM\Diverging_Smoke_Hilight.part
			Part :Dust_Ground_Spinning.part
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Hips
			Type Local
			EName CoreGlow
			Part ENEMYFX\Praetorian\MoM\Diverging_Smoke.part
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 15

		Event
			At Hips
			Type Local
			EName Glow
			Part ENEMYFX\Praetorian\MoM\Build_Up_Beam_Core.part
			LifeSpan 160
		End

		Event
			At Root
			Type Local
			EName GroundGlow
			Part :Build_Up_Beam_CoreFlat.part
			LifeSpan 160
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Hips
			Type Local
			EName SecParticles
			Part ENEMYFX\Praetorian\MoM\Build_Up_Scatter_Glows_01.part
			LifeSpan 100
		End

		Event
			At Hips
			Type Local
			EName Shaker
			Bhvr EnemyFx/DevouringEarth/Hamidon/Nucleus_Death/Electron_Camera_Shake.bhvr
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 90

		Event
			At Hips
			Type Local
			EName Hydra_Head_Explosion
			Part ENEMYFX\Praetorian\MoM\Death_Flash_Huge_01.part
			Part ENEMYFX\Praetorian\MoM\Build_Up_Scatter_Beams_02.part
			LifeSpan 100
		End

	End

	Condition
		On Time
		Time 100

		Event
			At Root
			Type Local
			EName Flash
			Part :Death_Flash_Huge_02.part
			LifeSpan 100
		End

	End

End
