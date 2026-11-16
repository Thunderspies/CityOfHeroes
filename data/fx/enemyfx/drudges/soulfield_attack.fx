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
		Type	Local
		At	Root
		Sound 	CoL_RadiantBurst_Attack_01 100 100 .8
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
		Part	:StaffEnergyDots_Soul.part
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
		Part	:StaffEnergyDots2_Soul.part
		Flags	Oneatatime
                While   WEAPON
		Lifespan 30
	End
End

#############################################################
End