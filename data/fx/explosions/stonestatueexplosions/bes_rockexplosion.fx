#########################################################
##	onfire
##
FxInfo  

LifeSpan	1150


Condition
	Time 	0

	Event
		Ename	Hookup
		Type	Local
		At	root

		Geom	obelisk_Hookup

	End

End

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	13

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosion.Fx

	End


####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	2

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosion.Fx

	End


############################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	7

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosion.Fx

	End

End


#############################################################################

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	9

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOpposite.Fx

	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	10

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOpposite.Fx

	End
	
############################################################
##TopRocks
####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	3

		ChildFx	Explosions\StonestatueExplosions\Top_Bes_RockExplosionLower.Fx

	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	8

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion2Opposite.Fx

	End

End

