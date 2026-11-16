#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time	0

	Event
		EName	Prime
		Type	Start
		At	Root
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			InitialVelocity	0 0.3 0.0
			InitialVelocityJitter	0.25 0.25 0.0
			StartJitter	5.0 2.0 5.0
			PositionOffset	0.0 0.0 1.0
			TrackRate	1.0

		End
		Magnet	T_Head
		LookAt  T_Head
		Lifespan 80
	End

	Event
		Ename 	Spinner1
		Type	Local
		At	Prime
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter	0.25 0.25 0.25
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner1
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part

		Part	:Soul_Head.part

		Lifespan 80
	End
End

#########################################################

End