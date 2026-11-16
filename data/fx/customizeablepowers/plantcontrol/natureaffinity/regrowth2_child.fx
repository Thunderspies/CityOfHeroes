#
FxInfo

	LifeSpan 45


###########################################################

	Condition
		On Cycle
		Time 20
		random 1

		Event
			At Origin
			Type Local
			Bhvr behaviors\fadeInOut.bhvr
			BhvrOverride
				PyrRotate 0 0 0
				PyrRotateJitter 8 6 60
				Scale 1 1 1
				#FadeOutLength 50
			End
			Geom FX_WindStream
			LifeSpan 40
		End

		Event
			At Origin
			Type Local
			Bhvr behaviors\fadeInOut.bhvr
			BhvrOverride
				PyrRotate 0 0 0
				PyrRotateJitter 8 6 60
				Scale -1 1 1
				#FadeOutLength 50
			End
			Geom FX_WindStream
			LifeSpan 40
		End

	End

End
