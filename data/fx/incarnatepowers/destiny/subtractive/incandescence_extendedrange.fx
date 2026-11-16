#                                                          
FxInfo

	Flags InheritAnimScale

	LifeSpan 100


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			ChildFx IncarnatePowers/Incarnate_Gather_Local.fx
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Hips
			Type Local
			EName TeleportOut
			BhvrOverride
				#PositionOffset 0 1 0
			End
			#Part :Incandescence_TeleportOut_Crackle.part
			Part :Incandescence_TeleportOut_Dust.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 33

		Event
			At Root
			Type Start
			EName BeamTarget
			BhvrOverride
				PositionOffset 0 -10 0
			End
		End

		Event
			At Root
			Type Start
			EName BeamStartOffset
			LookAt BeamTarget
			POther BeamTarget
			BhvrOverride
				PositionOffset 0 40 0
			End
			Part :Incandescence_TeleportIn_BeamDown.part
		End

	End

	Condition
		On Time
		Time 36

		Event
			At Root
			Type Start
			EName BeamFlash
			Part :Incandescence_TeleportIn_BeamFlash.part
			Part :Incandescence_TeleportIn_GroundFlash.part
			Part :Incandescence_Glow.part
			Part :Incandescence_Glow_Flat.part
			Part :Incandescence_Shockwave60.part
			Part :Incandescence_Shockwave60_Flat.part
			Part :Incandescence_FlashRing60.part
			Part :Incandescence_FlashRing60_Flat.part
		End

		Event
			At Root
			Type Start
			EName DustExpander
			Bhvr :DustRing_Expander.bhvr
			BhvrOverride
				PositionOffset 0 2 0
			End
			Part :Incandescence_DustRing60.part
			#Part :Incandescence_DotRing60.part
			LifeSpan 18
		End

	End

	Condition
		On Time
		Time 34

		Event
			At Hips
			Type StartPositOnly
			Bhvr :DustRing_Expander.bhvr
			BhvrOverride
				PyrRotateJitter 1 5 1
				EndScale 5 1 5
			End
			Part :Incandescence_Rings60.part
			LifeSpan 6
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName TeleportOut
			BhvrOverride
				PositionOffset 0 1 0
			End
			Part :Incandescence_SafeZone_60Flat01.part
			Part :Incandescence_SafeZone_60Flat02.part
			LifeSpan 10
		End

		Event
			At Root
			Type Start
			EName DustExpander
			Bhvr :DustRing_Expander.bhvr
			BhvrOverride
				PositionOffset 0 2 0
			End
			#Part :Incandescence_DotRing30.part
			LifeSpan 18
		End

	End

End
