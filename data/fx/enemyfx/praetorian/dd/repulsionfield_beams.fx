#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

#		Event
#			At Origin
#			Type Local
#			EName BeamA
#			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow.part
#			Part WORLD\Lairs\cot\Oranbega_Altar\Beam_BaseGlow_BottomFlat.part
#			Part WORLD\Lairs\cot\Oranbega_Altar\Beam.part
#		End

		Event
			At Root
			Type Start
			EName beam1
			BhvrOverride
				PositionOffset 6.1 6 6.1
			End
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow.part
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow_BottomFlat.part
			Part ENEMYFX\Praetorian\DD\Beam.part
		End

		Event
			At Root
			Type Start
			EName beam2
			BhvrOverride
				PositionOffset -6.10 6 -6.10
			End
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow.part
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow_BottomFlat.part
			Part ENEMYFX\Praetorian\DD\Beam.part
		End

		Event
			At Root
			Type Start
			EName beam3
			BhvrOverride
				PositionOffset 6.1 6 -6.10
			End
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow.part
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow_BottomFlat.part
			Part ENEMYFX\Praetorian\DD\Beam.part
		End

		Event
			At Root
			Type Start
			EName beam4
			BhvrOverride
				PositionOffset -6.10 6 6.1
			End
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow.part
			Part ENEMYFX\Praetorian\DD\Beam_BaseGlow_BottomFlat.part
			Part ENEMYFX\Praetorian\DD\Beam.part
		End

	End

End
