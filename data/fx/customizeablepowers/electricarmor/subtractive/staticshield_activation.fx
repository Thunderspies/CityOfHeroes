#############################################################
## StaticShield_Activation.FX
#############################################################

FxInfo

Lifespan 160

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound staticbuildup1 70 70 0.6
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	WepR
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		LifeSpan 30
	End

	Event
		Type	local
		At	WepL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	local
		At	HandL
		Part	:ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		Type	local
		At	HandR
		Part	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	Head
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	Chest
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	LarmL
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	LarmR
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	WepL
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	WepR
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	LLegR
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	LLegL
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	FootL
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

Condition
	On 	Time
	Time	20

	Event
		Type	Local
		At	FootR
		ChildFX	:Child_ElectricalArc.fx
		Lifespan 40
	End
End

#############################################################

End