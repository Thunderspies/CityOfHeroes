#########################################################
##	template

FxInfo


Lifespan 300

#########################################################
####################### AUDIO ###########################
#########################################################

Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
		Sound teletrans4 80 80 .88
	End

End

Condition

	On 	Time
	Time	52

	Event
		Type Local
		At origin
#		Sound	teletrans3 80 30 .78
	End

End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	2

	Event
		ENAME	NeckNode
		Type	Start
		At	Neck


	End

	Event
		ENAME	ChestGlow
		Type	Start
		At	Hips
		bhvroverride
			HueShift	100
		End

		Part	CustomizablePowers_Pools\Teleportation\Subtractive\TSChestGlow01.part
		Pmagnet NeckNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	ChestNode
		Type	Start
		At	Chest

	End

	Event
		ENAME	HeadGlow
		Type	Start
		At	Hair
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet ChestNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LarmRNode
		Type	Start
		At	LarmR

	End

	Event
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmRnode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LarmLNode
		Type	Start
		At	LarmL

	End

	Event
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LleglNode
		Type	Start
		At	Llegl

	End

	Event
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	LlegRNode
		Type	Start
		At	LlegR

	End

	Event
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0

	End


End

#########################################################
#################### BODY GLOW 2 ########################
#########################################################

Condition
	On	Time
	Time	52

	Event
		ENAME	ChestGlow
		Type	Start
		At	Hips
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet Neck
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Start
		At	Hair
		bhvroverride
			HueShift	100
		End

		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet Chest
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightUarmGlow
		Type	Start
		At	SpadR
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLarmGlow
		Type	Start
		At	WepR
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftUarmGlow
		Type	Start
		At	SpadL
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Start
		At	WepL
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LarmL
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftULegGlow
		Type	Start
		At	UlegL
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Start
		At	FootL
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet Llegl
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Start
		At	UlegR
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Start
		At	FootR
		bhvroverride
			HueShift	100
		End
		part1	CustomizablePowers_Pools\Teleportation\Subtractive\TSHeadGlow01.part
		Pmagnet LlegR
		Bhvr	Behaviors/TelportationFade2.bhvr
		Lifespan 0

	End


End

#########################################################

End