#############################################################
## ThrowSpines_Attack.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Origin
		Sound Quilltoss 70 70 0.7
	End
End

#############################################################

Condition
	On	Time
	Time	17

	Event
		EName	Prime
		Type	start
		At	WepL
		Bhvr	CustomizeablePowers\Spines\Spines\Quillprojectile3.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_SpinesCone
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	1
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	2
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	3
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	4
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	5
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	6
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	7
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	8
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	9
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	10
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	11
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	12
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	13
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecksStreak.part
		Part	CustomizeablePowers\Spines\Spines\DarkCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\LightCloudSpecks.part
		Part	CustomizeablePowers\Spines\Spines\GreenBits.part
		Part	CustomizeablePowers\Spines\Spines\BlackBits.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	Local
		At	chest
		Sound Quills6 70 70 0.7
	End

	Event
		Ename	All
		Type	Destroy
	End
End

#############################################################

End