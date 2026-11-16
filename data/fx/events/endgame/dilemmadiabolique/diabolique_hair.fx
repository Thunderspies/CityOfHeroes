#
FxInfo

	Flags DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			Ename HeadAnchor
			At Head
			Type Local
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.35 0
			End
			Part :Diabolique_Hair_TendrilGlow.part
			Part :Diabolique_Hair_TendrilSmoke2.part
			Part :Diabolique_Hair_TendrilSmoke4.part
		End

		Event
			At HeadAnchor
			Type PositOnly
			POther 0
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.25 0
			End
			Part :Diabolique_Hair_TendrilSmoke.part
		End

		Event
			At LArmL
			Type PositOnly
			EName LArmLFX
			POther HandL
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Mist_Cold_Detached.part
			#Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At LArmR
			Type PositOnly
			EName LArmRFX
			POther HandR
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Mist_Cold_Detached.part
			#Part :Mist_Big.part
			Part :Mist_Big_Trail.part
		End

		Event
			At HandL
			Type PositOnly
			EName LArmLFX
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Diabolique_Hair_TendrilSmoke4.part
		End

		Event
			At HandR
			Type PositOnly
			EName LArmRFX
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Mist_Cold_Detached.part
			Part :Mist_Big.part
			Part :Diabolique_Hair_TendrilSmoke4.part
		End

	End

End
