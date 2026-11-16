#############################################################
## Lightning_Red.fx
#############################################################

FxInfo

LifeSpan 165

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	TeslaOffset
		Type	start
		At	T_Root
		BhvrOverride
			PositionOffset		0 4.5 0
		End
		Sound AVBeam 125 125 1
	End

	Event
		Ename	ChargedBolt6
		Type	Start
		At	Chest
		Part	:Beam.part
		Part	:BeamCore.part
		POther	TeslaOffset
		Lifespan 70
	End
End

Condition
	On	Time
	Time	10

	Event
		Ename	ChargedBolt6
		Type	start
		At	Chest
		Part	:LightningSmall.Part
		Part	:LightningBig.Part
		Part	:LightningMedium.Part
		POther	TeslaOffset
		Lifespan 20
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		ChildFX	ENEMYFX\5thColumn\Portal\BodyGlow.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	22

	Event
		Type	Start
		At	Chest
		Part	:Embers.part
		Part	:Sparks.part
		Part	:Steam.part
		PMagnet	TeslaOffset
	End
End

#############################################################

End