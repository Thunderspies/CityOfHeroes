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
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegL
	End

	Event
		EName	Whip_Hind_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootL
	End

	Event
		EName	Whip_Hind_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeL
	End

	Event
		EName	Whip_Hind_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeL
	End

	Event
		EName	Whip_Hind_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ULegR
	End

	Event
		EName	Whip_Hind_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegR
	End

	Event
		EName	Whip_Hind_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_FootR
	End

	Event
		EName	Whip_Hind_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_ToeR
	End
	Event
		EName	Whip_Fore_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegL
	End

	Event
		EName	Whip_Fore_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegL
	End

	Event
		EName	Whip_Fore_FootL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootL
	End

	Event
		EName	Whip_Fore_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeL
	End

	Event
		EName	Whip_Fore_ToeL
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeL
	End

	Event
		EName	Whip_Fore_ULegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ULegR
	End

	Event
		EName	Whip_Fore_LLegR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_LLegR
	End

	Event
		EName	Whip_Fore_FootR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_FootR
	End

	Event
		EName	Whip_Fore_ToeR
		Type	Local
		At	WhipAnim
		AnimPiv	Fore_ToeR
	End
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	PositOnly
		At	Whip_Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_LLegL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	PositOnly
		At	Whip_Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_FootL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	PositOnly
		At	Whip_Hind_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_FootL
		Lifespan 7
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	PositOnly
		At	Whip_Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_ULegR
		Lifespan 7
	End
End


Condition
	On	Time
	Time	9

	Event
		Type	PositOnly
		At	Whip_Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_LLegR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	11

	Event
		Type	PositOnly
		At	Whip_Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_FootR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	PositOnly
		At	Whip_Hind_FootR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_ToeR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	PositOnly
		At	Whip_Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Hind_ToeR
		Lifespan 7
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_LLegL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	19

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_FootL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		POther	Whip_Fore_ToeL
		Lifespan 5
	End
End

Condition
	On	Time
	Time	23

	Event
		Type	PositOnly
		At	Whip_Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WhipAnim
		AnimPiv	Hips
		Part	V_COV\Powers\DemonSummoning\WhipCord_Handle.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		POther	Whip_Hind_ULegL
		Lifespan 37
	End

	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	WhipAnim
		AnimPiv Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_LLegL
		Lifespan 37
	End

	Event
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootL
		Lifespan 37
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Whip_Hind_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeL
		Lifespan 35

	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ULegR
		Lifespan 35
	End
End

Condition
	On	Time
	Time	11

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part

		POther	Whip_Hind_LLegR
		Lifespan 33

	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootR
		Lifespan 33
	End
End

Condition
	On	Time
	Time	14

	Event
		Type	Local
		At	Whip_Hind_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeR
		Lifespan 31
	End

	Event
		Type	Local
		At	Whip_Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegL
		Lifespan 31
	End
End

Condition
	On	Time
	Time	17

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_LLegL
		Lifespan 29
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_FootL
		Lifespan 29
	End
End

Condition
	On	Time
	Time	21

	Event
		Type	Local
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ToeL
		Lifespan 27
	End

	Event
		Type	Local
		At	Whip_Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegR
		Lifespan 27
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	Whip_Fore_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_LLegR
		Lifespan 25
	End

	Event
		Type	Local
		At	Whip_Fore_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord_Tip.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_FootR
		Lifespan 25
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	Whip_Fore_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord_Tip.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ToeR
		Lifespan 23
	End
End

Condition
	On	Time
	Time	35


	Event
		Type	PositOnly
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Trail.part
		Lifespan 12
	End
End

Condition
	On	Time
	Time	41

	Event
		Type	PositOnly
		At	Whip_Fore_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Sparks.part
		Lifespan 12

	End
End

Condition
	On 	Time
	Time 	40.5

	Event
		Type	PositOnly
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Extinguish.part
		Lifespan 10
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke.part
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke_Linger.part
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Extinguish.part
		Lifespan 8
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke.part
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke_Linger.part
		Part	V_COV\Powers\DemonSummoning\Whip_Sparks.part
		Lifespan 10
	End

	Event
		Type	PositOnly
		At	Whip_Fore_ULegR
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke.part
		Part	V_COV\Powers\DemonSummoning\Whip_Smoke_Linger.part
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Extinguish_Large.part
		Lifespan 6
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
	Time 	35

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 0.75
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	Origin
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