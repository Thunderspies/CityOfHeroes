#############################################################
## HellfireSmash_Activation.fx
#############################################################

FxInfo
Lifespan 60

#########################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	PositOnly
		At	UArmL
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		Part	:Sparks_Hellfire.part
		POther	LArmL
		LifeSpan	20
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		Part	:Sparks_Hellfire.part
		POther	LArmR
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	PositOnly
		At	LArmL
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		POther	WepL
		LifeSpan	20
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		POther	WepR
		LifeSpan	20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	WepL
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		LifeSpan	20
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:HellfireSmash_ArmFire.part
		Part	:HellfireSmash_ArmFire_Highlight.part

		LifeSpan	20
	End
End

#########################################################

End
