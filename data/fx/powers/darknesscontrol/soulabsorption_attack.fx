#
FxInfo
Lifespan 90

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_SoulAbsorption_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			EName Shockwave
			Part POWERS\DarknessControl\Tendrils_Core_Flat_Subtractive.part
			Part :Tendrils_Core_Flat.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Start
			EName Shockwave
			Part :Tendrils_Growing.part
			Part POWERS\DarknessControl\Tendrils_Growing_Core.part
			Part :Tendrils_Growing2.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Start
			EName Shockwave
			Part :Tendrils_Shriking.part
			Part :Tendrils_Core_Flat_Shrinking.part
			Part POWERS\DarknessControl\Tendrils_Core_Flat_Shrinking_Subtractive.part
			Part :Rings_Shrinking.part
			LifeSpan 55
		End

	End

End
