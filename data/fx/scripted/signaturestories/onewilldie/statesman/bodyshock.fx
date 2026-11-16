#                                                          
FxInfo


###########################################################

	Condition
		On Cycle
		Time 30

		Event
			At Chest
			Type Start
			ChildFx :CHILD_BodyShock.fx
			POther Head
			LifeSpan 31
		End

	End

	Condition

		Event
			At Chest
			Type Posit
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Chest_Rays.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Chest_Rays_Detached.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Chest_Flash.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Electricity_Surrounding.part
		End

		Event
			At Eyes
			Type Local
			EName EyeAnchor
			Geom CircleOfThorns
		End

		Event
			AltPiv 1
			At EyeAnchor
			Type Local
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Glow_Eye.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Rays_Eye.part
		End

		Event
			AltPiv 2
			At EyeAnchor
			Type Local
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Glow_Eye.part
			Part Scripted\SignatureStories\OneWillDie\Statesman\BodyShock_Rays_EyeR.part
		End

	End

End
