#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 150

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
		EName	Whip_Hind_LLegL
		Type	Local
		At	WhipAnim
		AnimPiv	Hind_LLegL
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
	Time	0

	Event
		Type	Local
		At	Whip_Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_LLegL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_FootL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ULegR
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_LLegR
		Lifespan 15
	End


	Event
		Type	Local
		At	Whip_Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_FootR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Hind_FootR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Fore_FootL
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Fore_ToeL
		Lifespan 13
	End
End

#############################################################

Condition
	On	Time
	Time	10
	Event
		EName	Whip_Hind_ULegL
		Type	Local
		At	Whip_Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_LLegL
		Lifespan 90
	End


	Event
		Type	Local
		At	Whip_Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootL
		Lifespan 90

	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Whip_Hind_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeL
		Lifespan 90

	End


	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ULegR
		Lifespan 90

	End
End

Condition
	On	Time
	Time	16

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_LLegR
		Lifespan 90

	End

	Event
		Type	Local
		At	Whip_Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_FootR
		Lifespan 90
	End
End


Condition
	On	Time
	Time	19

	Event
		Type	Local
		At	Whip_Hind_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Hind_ToeR
		Lifespan 85
	End

	Event
		Type	Local
		At	Whip_Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegL
		Lifespan 85
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Local
		At	Whip_Fore_ULegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_LLegL
		Lifespan 85
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_FootL
		Lifespan 85
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ToeL
		Lifespan 70
	End



	Event
		Type	Local
		At	Whip_Fore_ToeL
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		POther	Whip_Fore_ULegR
		Lifespan 70
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Whip_Fore_ULegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part

		POther	Whip_Fore_LLegR
		Lifespan 70
	End



	Event
		Type	Local
		At	Whip_Fore_LLegR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		POther	Whip_Fore_FootR
		Lifespan 70
	End
End

Condition
	On	Time
	Time	31

	Event
		Type	Local
		At	Whip_Fore_FootR
		Part	V_COV\Powers\DemonSummoning\WhipCord.part
		Part	V_COV\Powers\DemonSummoning\WhipFire.part
		Part	V_COV\Powers\DemonSummoning\WhipCord_Glow_Buff.part
		POther	Whip_Fore_ToeR
		Lifespan 50
	End
End

#############################################################

Condition
	On	Time
	Time	110

	Event
		Type	Local
		At	Whip_Hind_ULegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_LLegL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_FootL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeL
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_ToeL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ULegR
		Lifespan 15
	End

	Event
		Type	Local
		At	Whip_Hind_ULegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_LLegR
		Lifespan 15
	End


	Event
		Type	Local
		At	Whip_Hind_LLegR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_FootR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Hind_ToeR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Hind_FootR
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Hind_ToeR
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Fore_LLegL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Fore_FootL
		Lifespan 13
	End

	Event
		Type	Local
		At	Whip_Fore_FootL
		Part	V_COV\Powers\DemonSummoning\Whip_Fire_Stationary_Overhand.part
		POther	Whip_Fore_ToeL
		Lifespan 13
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