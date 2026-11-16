#############################################################
## Radiant_Burst.fx
#############################################################

FxInfo
Lifespan 60
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	PsionicRigging
		Sound Scare_01 88 88 .9
	End

	Event
		EName 	KeetsiePwns1
		Type	Local
		At	WepR
		Geom	CoL_Staff_01
		Flags	Oneatatime
       #        While   WEAPON
	End

	Event
		EName 	KeetsiePwns2
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Part	:StaffEnergyDots_Haunt.part
		Flags	Oneatatime
                While   WEAPON
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	KeetsiePwns3
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Part	:StaffEnergyDots2_Haunt.part
		Flags	Oneatatime
                While   WEAPON
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	KeetsiePwns4
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Part	:RadiantFlare_Haunt.part
		Flags	Oneatatime
                While   WEAPON
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Ename 	ScreenFlash
		Type	Start
		Part	:Radiant_Flash_Haunt.part
	End
End
#############################################################
End