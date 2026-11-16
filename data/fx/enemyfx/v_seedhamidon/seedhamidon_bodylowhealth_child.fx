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
			StartJitter 5 5 5
		End
		Part	:Splash_Small_Flash.part
		Part	:TailSplash_Small.part
		Part	:TailSplash_Small_Hilite.part
		Lifespan 25
	End
End

#############################################################

End