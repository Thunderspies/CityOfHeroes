#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start
		At	Origin
		Bhvr	Behaviors\Fastprojectile6.bhvr
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Magnet	Target
		Lookat	Target
	End
End

## ON HIT ###########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End