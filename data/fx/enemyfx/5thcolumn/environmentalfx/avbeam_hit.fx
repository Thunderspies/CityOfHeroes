#############################################################
## Lightning_Red.fx
#############################################################

FxInfo
LifeSpan 165

#############################################################


Condition
	On	Time
	Time	10

	Event

		Ename	ChargedBolt6
		Type	start
		At	node05
		altpiv	1
		Part	:LightningSmall.Part
		Part	:LightningBig.Part
		Part	:LightningMedium.Part
		POther	 Target
		Lifespan 20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Target
		ChildFX	ENEMYFX\5thColumn\Portal\BodyGlow.fx
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
		PMagnet	Root #node05
	End

#############################################################

End