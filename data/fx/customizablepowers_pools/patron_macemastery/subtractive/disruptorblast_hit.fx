#########################################################
##
##

FxInfo

LifeSpan	20

Input
	InpName	head
End

Input
	InpName	chest
End

Input
	InpName	UArmL
End

Input
	InpName	UArmR
End

##################################
Condition
	On 	Time
	Time 	0

	Event

		EName	GasSpurts
		Type	Local
		At	chest

		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\PoisonSpark.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\PoisonSpark2.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\PoisonSparkDark.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\PoisonSpark2Dark.part
		Sound policebothit1 80 80 .95

	End

	Event

		EName	Gas
		Type	Local
		At	chest

		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHitRed.part
		Part1	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHit.part
		Part2	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasSpecksHit.part
		Part3	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBubbles.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaks.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaksInvert.part
	End

	Event

		EName	Gas1
		Type	Local
		At	UArmL

		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHitRed.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHit.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasSpecksHit.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBubbles.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaks.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaksInvert.part


	End

	Event

		EName	Gas2
		Type	Local
		At	UArmR

		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHitRed.part
		Part1	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHit.part
		Part2	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasSpecksHit.part
		Part3	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBubbles.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaks.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBackstreaksInvert.part

	End

	Event

		EName	HeadConfusionClouds
		Type	Local
		At	Head

		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHeadRed.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHitRed.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasstreaks.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasHead.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasSpecksHead.part
		Part	CustomizablePowers_Pools\Patron_MaceMastery\Subtractive\MustardGasBubblesHead.part
	End

End


Condition
	On 	Time
	Time 	7

	Event

		EName	Gas
		Type	destroy

	End

	Event

		EName	Gas1
		Type	destroy

	End

	Event

		EName	Gas2
		Type	destroy

	End

End

Condition
	On 	Time
	Time 	15

	Event

		EName	HeadConfusionClouds
		Type	destroy

	End

End

Condition
	On 	Time
	Time 	5

	Event

		EName	GasSpurts
		Type	destroy

	End


End


End