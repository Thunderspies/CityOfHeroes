#########################################################
##	template

FxInfo


Input
	Inpname Chest

End

LifeSpan 15

Condition
	On Time
	Time 6

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:HolyDamage_LightRays.part
		Part	:HolyDamage_LightRaysUp.part
		Part	:HolyDamage_Circle.part
	End

End

Condition
	On 	Time
	Time	30

	Event
		Ename	Prime
		Type	Destroy
	End

End


End

