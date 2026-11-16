#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :Smoke_Aura_Fast.part
		End

		Event
			At Head
			Type Local
			BhvrOverride
				PositionOffset 0 0.1 0
			End
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Rings_Out.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Rings_Out2.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Rings_Out3.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Blobs.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Blob_Streaks.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Blob_Streaks_Extremes.part
			Part Scripted\SignatureStories\OneWillDie\SigArc6\Blob_Streaks_Detached.part
			Part :SmokeHead_Aura_Fast.part
		End

		Event
			At Eyes
			Type Local
			EName EyeAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.01 0.020 -0.070
			End
			Part :EyeGlow_Pupil.part
			Part :EyeGlow_BG.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.01 -0.05
			End
			Part :EyeGlow_Pupil.part
			Part :EyeGlow_BG.part
		End

	End

End
