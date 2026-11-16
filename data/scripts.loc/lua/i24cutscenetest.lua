cutsceneA = [====[
			{
			< 0 Camera
				Position Camera0
			>
			< 5 Actor1 "So, it ends like this, does it?"
			>
			< 0 Camera
				Position Camera1
			>
			< 5 Actor3 "You could've chose something different, General. But you ran, and this what happens."
			>
			< 0 Camera
				Position Camera0
			>
			< 5 Actor1 "Then let's end this."
			>
			< 3 FX
				Action GENERIC/Fades/Fade_5Sec.fx
			>
			]====]


cutsceneB = [====[
			< 0 Camera
				Position Camera0
			>
			< 5 Actor1 "So, it ends like this, does it?"
			>
			< 0 Camera
				Position Camera1
			>
			< 5 Actor3 "You could've chose something different, General. But you ran, and this what happens."
			>
			< 0 Camera
				Position Camera2
			>
			< 5 Actor2 "Not if I have anything to say about it."
			>
			< 0 Camera
				Position Camera0
			>
			< 3 Actor1 "Good to see you're on board, {HeroName}!"
			>
			< 3 FX
				Action GENERIC/Fades/Fade_5Sec.fx
			>
			]====]

cutsceneEnd = [====[
			< 0 Actor1 ""
					Action DestroyMe
			>
			< 0 Actor2 ""
					Action DestroyMe
			>
			< 0 Actor3 ""
					Action DestroyMe
			>
			< 0 Actor4 ""
					Action DestroyMe
			>
			< 0 Actor5 ""
					Action DestroyMe
			>
			< 0 Actor6 ""
					Action DestroyMe
			>
			< 0 Actor7 ""
					Action DestroyMe
			>
			< 0 Actor8 ""
					Action DestroyMe
			>
			< 0 Actor9 ""
					Action DestroyMe
			>
			< 0 Actor10 ""
					Action DestroyMe
			>
			< 0 Actor11 ""
					Action DestroyMe
			>
			< 0 Actor12 ""
					Action DestroyMe
			>
			< 0 Actor13 ""
					Action DestroyMe
			>
			< 0 Actor14 ""
					Action DestroyMe
			>
			< 0 Actor15 ""
					Action DestroyMe
			>
			< 0 Actor16 ""
					Action DestroyMe
			>
			< 0 Actor17 ""
					Action DestroyMe
			>
			< 0 Actor18 ""
					Action DestroyMe
			>
			< 1 Actor19 ""
					Action DestroyMe
			>
			}
			]====]

function tick()
	if Mission.ObjectiveIsSucceeded("ScriptNoHelp") then
		cutscene = cutsceneA .. cutsceneEnd
	else
		cutscene = cutsceneB .. cutsceneEnd
	end

	Encounter.StartCutSceneFromString(cutscene, Encounter.MyEncounter())
	EndScript()
end
