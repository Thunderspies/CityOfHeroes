#########################################################
##	
##

FxInfo
LifeSpan  100

##################################

Condition
	On Time
	Time 0

	Event
		Type	Posit
		At	uarmL 
		
		bhvroverride
			FadeOutLength		80
		end

		childfx	EnemyFx/Rikti/WhiteNoise_SweeperEnd.fx
		lookat	LarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		Type	Posit
		At	LarmL 
		
		bhvroverride
			FadeOutLength		80
		end

		childfx	EnemyFx/Rikti/WhiteNoise_SweeperEnd.fx
		lookat	handL
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	uarmR 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperEnd.fx
		lookat	LarmR
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	LarmR 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperEnd.fx
		lookat	handR
		bhvroverride
			stretch 1
		end
	End

############## Legs
	
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	ulegL 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	LlegL
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	LlegL 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	footL
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	ulegR 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	LlegR
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	LlegR 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	footR
		bhvroverride
			stretch 1
		end
	End
###################################################################################

	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	LlegL
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	ulegL 
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	footL
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	LlegL 
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	LlegR
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	ulegR 
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	footR
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	LlegR 
		bhvroverride
			stretch 1
		end
	End

###### torso

	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	head 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	hips
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	ulegR 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	uarmL
		bhvroverride
			stretch 1
		end
	End
	Event
		
		bhvroverride
			FadeOutLength		80
		end

		Type	Posit
		At	ulegL 
		childfx	EnemyFx/Rikti/WhiteNoise_SweeperLegs.fx
		lookat	uarmR
		bhvroverride
			stretch 1
		end
	End


End

end
