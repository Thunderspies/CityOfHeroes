#############################################################
## HEADER
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
		Geom	Quills
		Bhvr	behaviors\Quillprojectile3.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	1
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	2
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	3
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	4
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	5
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	6
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	7
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	8
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	9
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	10
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	11
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	12
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
	End

	Event
		EName	trail01
		Type	local
		At	Prime
		Altpiv	13
		Part	:LightCloudSpecksStreak.part
		Part	:DarkCloudSpecksStreak.part
		Part	:DarkCloudSpecks.part
		Part	:LightCloudSpecks.part
		Part4	:GreenBits.part
		Part5	:BlackBits.part
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