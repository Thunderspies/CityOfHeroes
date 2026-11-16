#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#Lifespan 200

#############################################################


Condition
	On 	Cycle
	Time 	15
	Chance	0.4

	Event
		Ename	HeadAnchor
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter 100 100 100
		End
		Part	:Splash_Small_Flash.part
		Part	:TailSplash_Small.part
		Part	:TailSplash_Small_Hilite.part
		Lifespan 20
	End
End

#############################################################

End