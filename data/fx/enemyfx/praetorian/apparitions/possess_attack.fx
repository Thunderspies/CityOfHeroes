#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_Possess_Attack_01 100 100 .8
	End

	Event
		EName	Prime
		Type	Start
		At	Root
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 0.0
			InitialVelocityJitter	0.0 0.25 0.0
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			TrackRate		0.8

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
			Spin 			0.0 0.4 0.0
			SpinJitter		0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner1
		BhvrOverride
			PositionOffset		0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End


Condition
	On 	Time
	Time	4

	Event
		EName	Prime2
		Type	Start
		At	Root
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 0.0
			InitialVelocityJitter	0.0 0.25 0.0
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			TrackRate		1.0

		End
		Magnet	T_Head
		LookAt  T_Head
		Lifespan 80
	End

	Event
		Ename 	Spinner2
		Type	Local
		At	Prime2
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter		0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner2
		BhvrOverride
			PositionOffset		0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End

Condition
	On 	Time
	Time	10

	Event
		EName	Prime3
		Type	Start
		At	Root
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 0.0
			InitialVelocityJitter	0.0 0.25 0.0
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			TrackRate		0.8

		End
		Magnet	T_Head
		LookAt  T_Head
		Lifespan 80
	End

	Event
		Ename 	Spinner3
		Type	Local
		At	Prime3
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter	0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner3
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		Part	:Soul_Trail_Core.part
		Part	:Soul_Trail_Large.part
		Part	:Soul_Head.part

		Lifespan 80
	End
End

Condition
	On 	Time
	Time	15

	Event
		EName	Prime4
		Type	Start
		At	Root
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			InitialVelocity		0 0.25 0.0
			InitialVelocityJitter	0.0 0.25 0.0
			StartJitter		5.0 2.0 5.0
			PositionOffset		0.0 0.0 1.5
			TrackRate		1.0

		End
		Magnet	T_Head
		LookAt  T_Head
		Lifespan 80
	End

	Event
		Ename 	Spinner4
		Type	Local
		At	Prime4
		BhvrOverride
			Spin 0.0 .4 0.0
			SpinJitter	0.2 0.2 0.2
		End
		Lifespan 80
	End

	Event
		Type	Local
		At	Spinner4
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