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

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosionFinal.Fx

	End


####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	2

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosionFinal.Fx

	End


############################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	7

		ChildFx	Explosions\StonestatueExplosions\Bes_RuneRockExplosionFinal.Fx

	End

End


#############################################################################

#####################################################################################3

Condition

	Event
		Type	start 
		At	Hookup
		Altpiv	9

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOppositeFinal.Fx

	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	10

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosionOppositeFinal.Fx

	End
	
############################################################
##TopRocks
####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	3

		ChildFx	Explosions\StonestatueExplosions\Top_Bes_RockExplosionLowerFinal.Fx

	End

####################################################

	Event
		Type	start 
		At	Hookup
		Altpiv	8

		ChildFx	Explosions\StonestatueExplosions\Bes_RockExplosion2OppositeFinal.Fx

	End

End

