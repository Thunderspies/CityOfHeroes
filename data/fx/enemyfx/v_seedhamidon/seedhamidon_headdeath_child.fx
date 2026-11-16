#############################################################
## SeedHamidon_Aura.fx
#############################################################

FxInfo
#Lifespan 200

#############################################################


Condition
	On 	Cycle
	Time 	15

	Event
		Ename	HeadAnchor
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Splash_Flash.part
		Part	:HeadSplash.part
		Part	:HeadSplash_Hilite.part
		Part	:Splash_Splatter_Hilight.part
		Part	:Splash_Splatter.part
		Lifespan 10
	End
End

#############################################################

End