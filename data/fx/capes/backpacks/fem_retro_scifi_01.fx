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
			Anim	FX_Backpack_Retro_SciFi_01
			BhvrOverride
				Scale			0.8 0.8 0.8
				PositionOffset		0 0.05 0.05
				PYRrotate 		-5 0 0

			End
		End


	End

	Condition
		On TriggerBits
		TriggerBits FLY
		DoMany 1

		Event
			EName JETFIRE
			At JETPACKANCHOR
			Type Local
			Flags OneAtATime
			While FLY
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 -0.9 -0.450000
			End
			Part :Idle_Fire_Add.part
			Part :Smoke_Ring.part
			Part :Idle_Fire.part
		End
	End


#############################################################

End