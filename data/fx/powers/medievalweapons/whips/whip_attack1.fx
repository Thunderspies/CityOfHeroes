#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 100

#############################################################
Condition
	On 	Time
	Time 	0

	Event
		Ename	WhipAnim
		Type	Local
		At	WepR
		Anim	CustomWeapon_Whip_01
		BhvrOverride
			Alpha	0
		End
	End

#############################################################
End

Condition
	On	Time
	Time	0
	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_LLegL
		Lifespan 55
	End

	Event
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootL
		Lifespan 55

	End
End

Condition
	On	Time
	Time	2

	Event
		EName	Whip_Hind_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeL
		Lifespan 55

	End

	Event
		EName	Whip_Hind_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ULegR
		Lifespan 55

	End
End

Condition
	On	Time
	Time	2

	Event
		EName	Whip_Hind_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_LLegR
		Lifespan 55

	End

	Event
		EName	Whip_Hind_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootR
		Lifespan 55

	End
End

Condition
	On	Time
	Time	4

	Event
		EName	Whip_Hind_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeR
		Lifespan 55

	End

	Event
		EName	Whip_Hind_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegL
		Lifespan 55

	End
End

Condition
	On	Time
	Time	6

	Event
		EName	Whip_Fore_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_LLegL
		Lifespan 55

	End

	Event
		EName	Whip_Fore_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_FootL
		Lifespan 55

	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Whip_Fore_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ToeL
		Lifespan 55

	End

	Event
		EName	Whip_Fore_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegR
		Lifespan 55

	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Whip_Fore_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_LLegR
		Lifespan 55

	End

	Event
		EName	Whip_Fore_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_FootR
		Lifespan 55

	End
End

Condition
	On	Time
	Time	8

	Event
		EName	Whip_Fore_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ToeR
		Lifespan 55


	End

	Event
		EName	Whip_Fore_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeR

	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Whip_Hind_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_FootR
		Lifespan 12

	End

	Event
		EName	Whip_Hind_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_ToeR
		Lifespan 12

	End

	Event
		EName	Whip_Hind_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_ULegL
		Lifespan 12

	End

	Event
		EName	Whip_Fore_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_LLegL
		Lifespan 12

	End

	Event
		EName	Whip_Fore_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_FootL
		Lifespan 12

	End

	Event
		EName	Whip_Fore_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_ToeL
		Lifespan 12

	End

	Event
		EName	Whip_Fore_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_ULegR
		Lifespan 12

	End

	Event
		EName	Whip_Fore_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part

		POther	Whip_Fore_LLegR
		Lifespan 12

	End

	Event
		EName	Whip_Fore_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part

		POther	Whip_Fore_FootR
		Lifespan 15

	End

	Event
		EName	Whip_Fore_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part

		POther	Whip_Fore_ToeR
		Lifespan 15

	End

	Event
		EName	Whip_Fore_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeR

	End
End

Condition
	On	Time
	Time	20


	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Trail.part
		Lifespan 40
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Trail.part
		Lifespan 35

	End
End

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Origin
		Sound Whooshbig 25.0 25.0 0.375
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 0.75
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	Origin
		#Sound	ArachnosPistol 100.0 30.0 1
		Sound ArachnosPistol 100.0 100.0 0.875
	End
End

#############################################################


Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	Origin
		Sound Whooshbig 100.0 100.0 0.25
	End
End

#############################################################

End