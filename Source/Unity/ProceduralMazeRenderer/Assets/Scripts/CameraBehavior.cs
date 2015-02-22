using UnityEngine;
using System.Collections;

public class CameraBehavior : MonoBehaviour {

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKey(KeyCode.W))
        {
            transform.Translate(new Vector3(0, 0, 0.1f), Space.Self);
        }
        if (Input.GetKey(KeyCode.A))
        {
            transform.Translate(new Vector3(-0.1f, 0, 0), Space.Self);
        }
        if (Input.GetKey(KeyCode.S))
        {
            transform.Translate(new Vector3(0, 0, -0.1f), Space.Self);
        }
        if (Input.GetKey(KeyCode.D))
        {
            transform.Translate(new Vector3(0.1f, 0, 0), Space.Self);
        }
        if (Input.GetKey(KeyCode.UpArrow))
        {
            transform.Rotate(Vector3.right, -1, Space.Self);
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            transform.Rotate(Vector3.up, -1, Space.Self);
        }
        if (Input.GetKey(KeyCode.DownArrow))
        {
            transform.Rotate(Vector3.right, 1, Space.Self);
        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
            transform.Rotate(Vector3.up, 1, Space.Self);
        }
	}

    void OnGUI()
    {
        if (GUI.Button(new Rect(0, 200, 100, 100), "Reset\nCamera\nRotation"))
        {
            this.transform.rotation = Quaternion.identity;
        }
        if (GUI.Button(new Rect(0, 300, 100, 100), "Move into\nmaze"))
        {
            MoveIntoMaze();
        }
    }

    private void MoveIntoMaze()
    {
        GameObject[] startSpaces = GameObject.FindGameObjectsWithTag("Start");
        if (startSpaces.Length == 0)
            return;
        Vector3 pos = startSpaces[Mathf.RoundToInt(Random.Range(0, startSpaces.Length))].transform.position;
        pos.y += 0.5f;
        this.transform.position = pos;
        this.transform.rotation = Quaternion.identity;
    }
}
