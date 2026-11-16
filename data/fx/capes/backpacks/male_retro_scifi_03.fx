#############################################################
## Costume Anim Entity - Backpack
#############################################################

FxInfo

Flags InheritAlpha DontSuppress //InheritAnimScale

#############################################################

	Condition
		On 	Time
		Time 	0

		Event
			EName 	JETPACKANCHOR
			Type	Local
			At	Back
			Anim	FX_Backpack_Retro_SciFi_02
			BhvrOverride
				Scale			1 1 1
				PositionOffset		0 0 0
	#			PYRrotate 		0 0 0
	# 			PYRrotateJitter 	0 0 0
			End
		End

		Event
			Type	Local
			At	Back
			Geom	Backpack_Retro_SciFi_02_Rings
			BhvrOverride
				Tintgeom 1
				Scale			1 1 1
				PositionOffset		0 0 0
			End
		End

		Event
			EName 	JETPACKOFFSETS
			Type	Local
			At	JETPACKANCHOR
			Geom	Retro_SciFi_02_FanOffset
			BhvrOverride
				Alpha 1
			End
		End
	End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			EName FANSPIN
			At JETPACKOFFSETS
			Altpiv 1
			Type Local
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 90 0 0
				PositionOffset 0 0 -0.03
			End
			Part :FanSpin.part
		End

		Event
			EName JETFIRE
			At JETPACKOFFSETS
			Altpiv 2
			Type Local
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.05 0
			End
			Part :Idle_Fire_Add.part
			Part :Smoke_Ring.part
			Part :Idle_Fire.part
		End

		Event
			EName JETFIRE2
			At JETPACKOFFSETS
			Altpiv 3
			Type Local
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.05 0
			End
			Part :Idle_Fire_Add.part
			Part :Smoke_Ring.part
			Part :Idle_Fire.part
		End
	End


#############################################################

End