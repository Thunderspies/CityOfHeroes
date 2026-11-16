#########################################################
##	onfire
##
FxInfo  

LifeSpan	1100


Condition

	Event
		Type	start 
		At	chest
		
		ChildFx	Explosions\WoodExplosion\Large_WoodExplosion.Fx

	End

End


Condition

	Event
		Type	start 
		At	chest
		BhvrOverride
			PositionOffset		0 2 0
		End
		ChildFx	Explosions\MetalExplosion\MetalExplosionChild.fx

	End

End

End